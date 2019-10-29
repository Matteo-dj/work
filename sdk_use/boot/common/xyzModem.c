// SPDX-License-Identifier: eCos-2.0
/*
 *==========================================================================
 *
 *      xyzModem.c
 *
 *      RedBoot stream handler for xyzModem protocol
 *
 *==========================================================================
 *#####DESCRIPTIONBEGIN####
 *
 * Author(s):    gthomas
 * Contributors: gthomas, tsmith, Yoshinori Sato
 * Date:         2000-07-14
 * Purpose:
 * Description:
 *
 * This code is part of RedBoot (tm).
 *
 *####DESCRIPTIONEND####
 *
 *==========================================================================
 */

#include <types.h>
#include <command.h>
#include <xyzModem.h>
#include <debug.h>
#include <boot.h>
#include "securec.h"

extern uint16_t crc16_ccitt(uint16_t crc_start, unsigned char *buf, int len);
/* Assumption - run xyzModem protocol over the console port */

/* Values magic to the protocol */
#define SOH 0x01
#define STX 0x02
#define EOT 0x04
#define ACK 0x06
#define BSP 0x08
#define NAK 0x15
#define CAN 0x18
#define EOF 0x1A		/* ^Z for DOS officionados */

#define USE_YMODEM_LENGTH

/* Data & state local to the protocol */
static struct
{
#ifdef REDBOOT
  hal_virtual_comm_table_t *__chan;
#else
  int *__chan;
#endif
  unsigned char pkt[1024], *bufp;
  unsigned char blk, cblk, crc1, crc2;
  unsigned char next_blk;	/* Expected block */
  unsigned char rsv[3];
  int len, mode, total_retries;
  int total_SOH, total_STX, total_CAN;
  bool crc_mode, at_eof, tx_ack;
#ifdef USE_YMODEM_LENGTH
  unsigned long file_length, read_length;
#endif
} xyz;

/*w00190206:115200��Ӧ1024�ֽڲ���Ҫ2s,��Ϊ1s*/
#define xyzModem_CHAR_TIMEOUT            1000	/* 1 seconds */
#define xyzModem_MAX_RETRIES             62
#define xyzModem_MAX_RETRIES_WITH_CRC    60
#define xyzModem_CAN_COUNT               3	/* Wait for 3 CAN before quitting */

u32 g_load_file_length = 0;

#ifndef REDBOOT			/*SB */
typedef int cyg_int32;
static int
CYGACC_COMM_IF_GETC_TIMEOUT (char chan, char *c)
{
#define DELAY 20
    unsigned long counter = 0;

    HI_UNREF_PARAM(chan);
    
    while (!serial_tstc () && (counter < xyzModem_CHAR_TIMEOUT * 1000 / DELAY))
    {
        udelay (DELAY);
        counter++;
    }
    if (serial_tstc ())
    {
      *c = (char)getc();
      return 1;
    }

    return 0;
}

static void
CYGACC_COMM_IF_PUTC (char x, char y)
{
    HI_UNREF_PARAM(x);
    putc (y);
}

/* Validate a hex character */
__inline__ static bool
_is_hex (char c)
{
    if (((c >= '0') && (c <= '9')) 
        || ((c >= 'A') && (c <= 'F')) 
        || ((c >= 'a') && (c <= 'f')))
    {
        return true;
    }
    else
    {
        return false;
    }

}

/* Convert a single hex nibble */
__inline__ static int
_from_hex (char c)
{
  int ret = 0;

  if ((c >= '0') && (c <= '9'))
    {
      ret = (c - '0');
    }
  else if ((c >= 'a') && (c <= 'f'))
    {
      ret = (c - 'a' + 0x0a);
    }
  else if ((c >= 'A') && (c <= 'F'))
    {
      ret = (c - 'A' + 0x0A);
    }
  return ret;
}

/* Convert a character to lower case */
__inline__ static char
_tolower (char c)
{
  if ((c >= 'A') && (c <= 'Z'))
    {
      c = (c - 'A') + 'a';
    }
  return c;
}

/* Parse (scan) a number */
static bool
parse_num (char *s, unsigned long *val, char **es, char *delim)
{
  bool first = true;
  int radix = 10;
  char c;
  unsigned long result = 0;
  int digit;

  while (*s == ' ')
    s++;
  while (*s)
    {
      if (first && (s[0] == '0') && (_tolower (s[1]) == 'x'))
	{
	  radix = 16;
	  s += 2;
	}
      first = false;
      c = *s++;
      if (_is_hex (c) && ((digit = _from_hex (c)) < radix))
	{
	  /* Valid digit */
#ifdef CYGPKG_HAL_MIPS
	  /* FIXME: tx49 compiler generates 0x2539018 for MUL which */
	  /* isn't any good. */
	  if (16 == radix)
	    result = result << 4;
	  else
	    result = 10 * result;
	  result += digit;
#else
	  result = (unsigned long)(((int)result * radix) + digit);
#endif
	}
      else
	{
	  if (delim != (char *) 0)
	    {
	      /* See if this character is one of the delimiters */
	      char *dp = delim;
	      while (*dp && (c != *dp))
		dp++;
	      if (*dp)
		break;		/* Found a good delimiter */
	    }
	  return false;		/* Malformatted number */
	}
    }
  *val = result;
  if (es != (char **) 0)
    {
/*
            �������ˡ�z00193780
            ������ˡ�c00233580
            �������������������κϷ��Լ�飬 �豣����
            ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/
      *es = s;
    }
  return true;
}

#endif

#define USE_SPRINTF
#ifdef DEBUG
#ifndef USE_SPRINTF
/*
 * Note: this debug setup only works if the target platform has two serial ports
 * available so that the other one (currently only port 1) can be used for debug
 * messages.
 */
static int
zm_dprintf (char *fmt, ...)
{
  int cur_console;
  va_list args;

  va_start(args, fmt);
#ifdef REDBOOT
  cur_console =
    CYGACC_CALL_IF_SET_CONSOLE_COMM
    (CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT);
  CYGACC_CALL_IF_SET_CONSOLE_COMM (1);
#endif
  diag_vprintf (fmt, args);
  va_end(args);

#ifdef REDBOOT
  CYGACC_CALL_IF_SET_CONSOLE_COMM (cur_console);
#endif
}

static void
zm_flush (void)
{
}

#else
/*
 * Note: this debug setup works by storing the strings in a fixed buffer
 */
#define FINAL
#ifdef FINAL
static char *zm_out = (char *) 0x00380000;
static char *zm_out_start = (char *) 0x00380000;
#else
static char zm_buf[8192];
static char *zm_out = zm_buf;
static char *zm_out_start = zm_buf;

#endif
static int
zm_dprintf (char *fmt, ...)
{
  int len;
  va_list args;

  va_start (args, fmt);
  len = vsprintf_s (zm_out, fmt, args);
  va_end(args);
  zm_out += len;
  return len;
}

static void
zm_flush (void)
{
#ifdef REDBOOT
  char *p = zm_out_start;
  while (*p)
    mon_write_char (*p++);
#endif
  zm_out = zm_out_start;
}
#endif

static void
zm_dump_buf (void *buf, int len)
{
#ifdef REDBOOT
  diag_vdump_buf_with_offset (zm_dprintf, buf, len, 0);
#else

#endif
}

static unsigned char zm_buf[2048];
static unsigned char *zm_bp;

static void
zm_new (void)
{
  zm_bp = zm_buf;
}

static void
zm_save (unsigned char c)
{
  *zm_bp++ = c;
}

static void
zm_dump (int line)
{
  zm_dprintf ("Packet at line: %d\n", line);
  zm_dump_buf (zm_buf, zm_bp - zm_buf);
}

#define ZM_DEBUG(x) x
#else
#define ZM_DEBUG(x)
#endif

/* Wait for the line to go idle */
static void
xyzModem_flush (void)
{
    int res = 0;
    char c = 0;

    for(;;)
    {
        res = CYGACC_COMM_IF_GETC_TIMEOUT ((char)(*xyz.__chan), &c);
        if (!res)
            return;
    }
}
/* return 0 if success */
static int xyzModem_get_hdr (void)
{
    char c = 0;
    int res;
    bool hdr_found = false;
    int i, can_total, hdr_chars;
    unsigned short cksum;

    ZM_DEBUG (zm_new ());
    /* Find the start of a header */
    can_total = 0;
    hdr_chars = 0;

    if (xyz.tx_ack)
    {
        CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), ACK);
        xyz.tx_ack = false;
    }
    while (!hdr_found)
    {
        res = CYGACC_COMM_IF_GETC_TIMEOUT ((char)(*xyz.__chan), &c);
        ZM_DEBUG (zm_save (c));
        if (res)
    	{
    	  hdr_chars++;
    	  switch (c)
    	    {
    	    case SOH:
                xyz.total_SOH++;
                //����ִ�У�����break                
    	    case STX:
                if (c == STX)
                xyz.total_STX++;
                hdr_found = true;
                break;
    	    case CAN:
                xyz.total_CAN++;
                ZM_DEBUG (zm_dump (__LINE__));
                if (++can_total == xyzModem_CAN_COUNT)
                {
                    return xyzModem_cancel;
                }
                else
                {
                    /* Wait for multiple CAN to avoid early quits */
                    break;
                }
            case EOT:
                /* EOT only supported if no noise */
                if (hdr_chars == 1)
                {
                    CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), ACK);
                    ZM_DEBUG (zm_dprintf ("ACK on EOT #%d\n", __LINE__));
                    ZM_DEBUG (zm_dump (__LINE__));
                    return xyzModem_eof;
                }
                break;
            default:
                /* Ignore, waiting for start of header */
                ;
    	    }
    	}
        else
    	{
            /* Data stream timed out */
            xyzModem_flush ();	/* Toss any current input */
            ZM_DEBUG (zm_dump (__LINE__));
            CYGACC_CALL_IF_DELAY_US ((cyg_int32) 250000);
            return xyzModem_timeout;
    	}
    }

  /* Header found, now read the data */
  res = CYGACC_COMM_IF_GETC_TIMEOUT ((char)(*xyz.__chan), (char *) &xyz.blk);
  ZM_DEBUG (zm_save (xyz.blk));
  if (!res)
    {
      ZM_DEBUG (zm_dump (__LINE__));
      return xyzModem_timeout;
    }
  res = CYGACC_COMM_IF_GETC_TIMEOUT ((char)(*xyz.__chan), (char *) &xyz.cblk);
  ZM_DEBUG (zm_save (xyz.cblk));
  if (!res)
    {
      ZM_DEBUG (zm_dump (__LINE__));
      return xyzModem_timeout;
    }
  xyz.len = (c == SOH) ? 128 : 1024;
  xyz.bufp = xyz.pkt;
  for (i = 0; i < xyz.len; i++)
    {
      res = CYGACC_COMM_IF_GETC_TIMEOUT ((char)(*xyz.__chan), &c);
      ZM_DEBUG (zm_save (c));
      if (res)
	{
	  xyz.pkt[i] = (unsigned char)c;
	}
      else
	{
	  ZM_DEBUG (zm_dump (__LINE__));
	  return xyzModem_timeout;
	}
    }
  res = CYGACC_COMM_IF_GETC_TIMEOUT ((char)(*xyz.__chan), (char *) &xyz.crc1);
  ZM_DEBUG (zm_save (xyz.crc1));
  if (!res)
    {
      ZM_DEBUG (zm_dump (__LINE__));
      return xyzModem_timeout;
    }
  if (xyz.crc_mode)
    {
      res = CYGACC_COMM_IF_GETC_TIMEOUT ((char)(*xyz.__chan), (char *) &xyz.crc2);
      ZM_DEBUG (zm_save (xyz.crc2));
      if (!res)
        {
            ZM_DEBUG (zm_dump (__LINE__));
            return xyzModem_timeout;
        }
    }
  ZM_DEBUG (zm_dump (__LINE__));
  /* Validate the message */
  if ((xyz.blk ^ xyz.cblk) != (unsigned char) 0xFF)
    {
      ZM_DEBUG (zm_dprintf
		("Framing error - blk: %x/%x/%x\n", xyz.blk, xyz.cblk,
		 (xyz.blk ^ xyz.cblk)));
      ZM_DEBUG (zm_dump_buf (xyz.pkt, xyz.len));
      xyzModem_flush ();
      return xyzModem_frame;
    }
  /* Verify checksum/CRC */
  if (xyz.crc_mode)
    {
        cksum = crc16_ccitt (0, xyz.pkt, xyz.len);
        if (cksum != ((xyz.crc1 << 8) | xyz.crc2))
        {
            ZM_DEBUG (zm_dprintf ("CRC error - recvd: %02x%02x, computed: %x\n",
            		xyz.crc1, xyz.crc2, cksum & 0xFFFF));
            print_msg2("CRC error:",xyz.crc1,xyz.crc2);
            return xyzModem_cksum;
        }
    }
  else
    {
      cksum = 0;
      for (i = 0; i < xyz.len; i++)
	{
	  cksum += xyz.pkt[i];
	}
      if (xyz.crc1 != (cksum & 0xFF))
	{
	  ZM_DEBUG (zm_dprintf
		    ("Checksum error - recvd: %x, computed: %x\n", xyz.crc1,
		     cksum & 0xFF));
	  return xyzModem_cksum;
	}
    }
  /* If we get here, the message passes [structural] muster */
  return 0;
}
/* if success return 0 */
int xyzModem_stream_open (connection_info_t * info, int *err)
{
#ifdef REDBOOT
    int console_chan;
#endif
    int stat = 0;
    int retries = xyzModem_MAX_RETRIES;
    int crc_retries = xyzModem_MAX_RETRIES_WITH_CRC;

/*    ZM_DEBUG(zm_out = zm_out_start); */
#ifdef xyzModem_zmodem
    if (info->mode == xyzModem_zmodem)
    {
        *err = xyzModem_noZmodem;
        return -1;
    }
#endif

#ifdef REDBOOT
    /* Set up the I/O channel.  Note: this allows for using a different port in the future */
    console_chan =
    CYGACC_CALL_IF_SET_CONSOLE_COMM
        (CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT);
    if (info->chan >= 0)
    {
        CYGACC_CALL_IF_SET_CONSOLE_COMM (info->chan);
    }
    else
    {
        CYGACC_CALL_IF_SET_CONSOLE_COMM (console_chan);
    }
    xyz.__chan = CYGACC_CALL_IF_CONSOLE_PROCS ();

    CYGACC_CALL_IF_SET_CONSOLE_COMM (console_chan);
    CYGACC_COMM_IF_CONTROL (*xyz.__chan, __COMMCTL_SET_TIMEOUT,
    	  xyzModem_CHAR_TIMEOUT);
#else
    /* TODO: CHECK ! */
    static int dummy;
    xyz.__chan = &dummy;
#endif
    xyz.len = 0;
    xyz.crc_mode = true;//false  changed by wuxianfeng
    xyz.at_eof = false;
    xyz.tx_ack = false;
    xyz.mode = info->mode;
    xyz.total_retries = 0;
    xyz.total_SOH = 0;
    xyz.total_STX = 0;
    xyz.total_CAN = 0;
#ifdef USE_YMODEM_LENGTH
    xyz.read_length = 0;
    xyz.file_length = 0;
#endif

    CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), (xyz.crc_mode ? 'C' : NAK));

    if (xyz.mode == xyzModem_xmodem)
    {
        /* X-modem doesn't have an information header - exit here */
        xyz.next_blk = 1;
        return 0;
    }

    while (retries-- > 0)
    {
        stat = xyzModem_get_hdr ();
        if (stat == 0)
    	{
        	/* Y-modem file information header */
            if (xyz.blk == 0)
            {
#ifdef USE_YMODEM_LENGTH
                /* skip filename */
                while (*xyz.bufp++)
                    ;
                /* get the length */
                parse_num ((char *) xyz.bufp, &xyz.file_length, (char **)HI_NULL, " ");
#ifndef TURNOFF_NEWFUNCTION_BEFORE_B020
				//��ȡ�ļ���С
				g_load_file_length = xyz.file_length;
#endif
#endif
                /* The rest of the file name data block quietly discarded */
                xyz.tx_ack = true;
            }
            xyz.next_blk = 1;
            xyz.len = 0;
            return 0;
    	}
        else if (stat == xyzModem_timeout)
    	{
            if (--crc_retries <= 0)
                xyz.crc_mode = false;
            CYGACC_CALL_IF_DELAY_US (5 * 100000);	/* Extra delay for startup */
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), (xyz.crc_mode ? 'C' : NAK));
            xyz.total_retries++;
            ZM_DEBUG (zm_dprintf ("NAK (%d)\n", __LINE__));
    	}
        if (stat == xyzModem_cancel)
        {
            break;
    	}
    }
    *err = stat;
    ZM_DEBUG (zm_flush ());
    return -1;
}

int xyzModem_stream_read (char *buf, int size, int *err)
{
    int stat, total, len;
    int retries;

    total = 0;
    stat = xyzModem_cancel;
    /* Try and get 'size' bytes into the buffer */
    while (!xyz.at_eof && (size > 0))
    {
        if (xyz.len == 0)
    	{
            retries = xyzModem_MAX_RETRIES;
            while (retries-- > 0)
            {
                stat = xyzModem_get_hdr ();
                if (stat == 0)
                {
                    if (xyz.blk == xyz.next_blk)
                    {
                        xyz.tx_ack = true;
                        ZM_DEBUG (zm_dprintf
                        ("ACK block %d (%d)\n", xyz.blk, __LINE__));
                        xyz.next_blk = (xyz.next_blk + 1) & 0xFF;

#if defined(xyzModem_zmodem) || defined(USE_YMODEM_LENGTH)
                        if (xyz.mode == xyzModem_xmodem || xyz.file_length == 0)
#endif
                        {
                            /* Data blocks can be padded with ^Z (EOF) characters */
                            /* This code tries to detect and remove them */
                            if ((xyz.bufp[xyz.len - 1] == EOF) &&
                                (xyz.bufp[xyz.len - 2] == EOF) &&
                                (xyz.bufp[xyz.len - 3] == EOF))
                            {
                                while (xyz.len
                                 && (xyz.bufp[xyz.len - 1] == EOF))
                                {
                                    xyz.len--;
                                }
                            }
                        }

#ifdef USE_YMODEM_LENGTH
                        /*
                        * See if accumulated length exceeds that of the file.
                        * If so, reduce size (i.e., cut out pad bytes)
                        * Only do this for Y-modem (and Z-modem should it ever
                        * be supported since it can fall back to Y-modem mode).
                        */
                        if (xyz.mode != xyzModem_xmodem && 0 != xyz.file_length)
                        {
                            xyz.read_length += (unsigned long)xyz.len;
                            if (xyz.read_length > xyz.file_length)
                            {
                                xyz.len -= (int)(xyz.read_length - xyz.file_length);
                            }
                        }
#endif
                        break;
                    }
                    else if (xyz.blk == ((xyz.next_blk - 1) & 0xFF))
                    {
                        /* Just re-ACK this so sender will get on with it */
                        CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), ACK);
                        continue;	/* Need new header */
                    }
                    else
                    {
                        stat = xyzModem_sequence;
                    }
                }
                if (stat == xyzModem_cancel)
                {
                    break;
                }
                if (stat == xyzModem_eof)
                {
                    CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), ACK);
                    ZM_DEBUG (zm_dprintf ("ACK (%d)\n", __LINE__));
                    if (xyz.mode == xyzModem_ymodem)
                    {
                        CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan),
                           (xyz.crc_mode ? 'C' : NAK));
                        xyz.total_retries++;
                        ZM_DEBUG (zm_dprintf ("Reading Final Header\n"));
                        stat = xyzModem_get_hdr ();
                        CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), ACK);
                        ZM_DEBUG (zm_dprintf ("FINAL ACK (%d)\n", __LINE__));
                    }
                    xyz.at_eof = true;
                    break;
                }
                CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), (xyz.crc_mode ? 'C' : NAK));
                xyz.total_retries++;
                ZM_DEBUG (zm_dprintf ("NAK (%d)\n", __LINE__));
            }
            if (stat < 0)
            {
                *err = stat;
                xyz.len = -1;
                return total;
            }
    	}
        /* Don't "read" data from the EOF protocol package */
        if (!xyz.at_eof)
        {
            len = xyz.len;
            if (size < len)
            len = size;
/*
                            �������ˡ�z00193780
                            ������ˡ�w00190206
                            �����������boot��memcpy��memset��strcpy�滻Ϊ��ȫ�ӿڻ�����2k bytes���ҵĴ���,�������Ͻӿ����ֱܷ�ή��һ�����������Խ���boot�²�����ȫ�ӿ��滻��
                            ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/
            memcpy_s(buf, (unsigned int)len, xyz.bufp, (unsigned int)len);
            size -= len;
            buf += len;
            total += len;
            xyz.len -= len;
            xyz.bufp += len;
        }
    }
    return total;
}

void xyzModem_stream_close (int *err)
{
    HI_UNREF_PARAM(err);
    ZM_DEBUG (zm_flush ());
}

/* Need to be able to clean out the input buffer, so have to take the */
/* getc */
void
xyzModem_stream_terminate (bool abort, int (*getchar) (void))
{
    if (abort)
    {
        ZM_DEBUG (zm_dprintf ("!!!! TRANSFER ABORT !!!!\n"));
        switch (xyz.mode)
        {
        /*
                        �������ˡ�z00193780
                        ������ˡ�b00208046
                        ����������������ӿڣ��÷�֧�豣�����豣����
                        ������Ӱ�졿��Ӱ������ҵ�����θ澯
        */        
        case xyzModem_xmodem:
        case xyzModem_ymodem:
            /* The X/YMODEM Spec seems to suggest that multiple CAN followed by an equal */
            /* number of Backspaces is a friendly way to get the other end to abort. */
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), CAN);
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), CAN);
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), CAN);
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), CAN);
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), BSP);
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), BSP);
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), BSP);
            CYGACC_COMM_IF_PUTC ((char)(*xyz.__chan), BSP);
            /* Now consume the rest of what's waiting on the line. */
            ZM_DEBUG (zm_dprintf ("Flushing serial line.\n"));
            xyzModem_flush ();
            xyz.at_eof = true;
            break;
#ifdef xyzModem_zmodem
        case xyzModem_zmodem:
            /* Might support it some day I suppose. */
            break;
#endif
        default:
            ;

        }
    }
    else
    {
        ZM_DEBUG (zm_dprintf ("Engaging cleanup mode...\n"));
        /*
                    * Consume any trailing crap left in the inbuffer from
                    * previous received blocks. Since very few files are an exact multiple
                    * of the transfer block size, there will almost always be some gunk here.
                    * If we don't eat it now, RedBoot will think the user typed it.
                    */
        ZM_DEBUG (zm_dprintf ("Trailing gunk:\n"));
        while (((*getchar) ()) > -1)
        ;
        ZM_DEBUG (zm_dprintf ("\n"));
        /*
                    * Make a small delay to give terminal programs like minicom
                    * time to get control again after their file transfer program
                    * exits.
                    */
        CYGACC_CALL_IF_DELAY_US ((cyg_int32) 250000);
    }
}

char *
xyzModem_error (int err)
{
  switch (err)
    {
    case xyzModem_access:
      return "Can't access file";
      //break;
    case xyzModem_noZmodem:
      return "Sorry, zModem not available yet";
      //break;
    case xyzModem_timeout:
      return "Timed out";
      //break;
    case xyzModem_eof:
      return "End of file";
      //break;
    case xyzModem_cancel:
      return "Cancelled";
      //break;
    case xyzModem_frame:
      return "Invalid framing";
      //break;
    case xyzModem_cksum:
      return "CRC/checksum error";
      //break;
    case xyzModem_sequence:
      return "Block sequence error";
      //break;
    default:
      return "Unknown error";
      //break;
    }
}

