#include <linux/rbtree.h>


#ifndef RB_AUGMENT
#define RB_AUGMENT(x)
#endif

#define RB_HEAD(root)        (root)->rb_node
#define RB_COLOR(node)        (node)->rb_color
#define RB_PARENT(node)        (node)->rb_parent
#define RB_RIGHT(node)        (node)->rb_right
#define RB_LEFT(node)        (node)->rb_left

#define RB_SET_BLACKRED(black, red) do {            \
    RB_COLOR(red) = RB_RED;                    \
    RB_COLOR(black) = RB_BLACK;                \
} while (0)


#define RB_SET(elm, parent) do {                \
    RB_COLOR(elm) = RB_RED;                \
    RB_PARENT(elm) = parent;                \
    RB_LEFT(elm) = RB_RIGHT(elm) = NULL;    \
} while (0)


#define RB_ROTATE_RIGHT(head, elm, node) do {            \
    (node) = RB_LEFT(elm);                    \
    if ((RB_LEFT(elm) = RB_RIGHT(node))) {            \
        RB_PARENT(RB_RIGHT(node)) = (elm);        \
    }                            \
    RB_AUGMENT(elm);                    \
    if ((RB_PARENT(node) = RB_PARENT(elm))) {        \
        if ((elm) == RB_LEFT(RB_PARENT(elm)))        \
            RB_LEFT(RB_PARENT(elm)) = (node);    \
        else                        \
            RB_RIGHT(RB_PARENT(elm)) = (node);    \
    } else                            \
        (head)->rb_node = (node);            \
    RB_RIGHT(node) = (elm);                    \
    RB_PARENT(elm) = (node);                    \
    RB_AUGMENT(node);                    \
    if ((RB_PARENT(node)))                    \
        RB_AUGMENT(RB_PARENT(node));            \
} while(0)


#define RB_ROTATE_LEFT(head, elm, node) do {            \
    (node) = RB_RIGHT(elm);                    \
    if ((RB_RIGHT(elm) = RB_LEFT(node))) {            \
        RB_PARENT(RB_LEFT(node)) = (elm);        \
    }                            \
    RB_AUGMENT(elm);                    \
    if ((RB_PARENT(node) = RB_PARENT(elm))) {        \
        if ((elm) == RB_LEFT(RB_PARENT(elm)))        \
            RB_LEFT(RB_PARENT(elm)) = (node);    \
        else                        \
            RB_RIGHT(RB_PARENT(elm)) = (node);    \
    } else                            \
        (head)->rb_node = (node);            \
    RB_LEFT(node) = (elm);                    \
    RB_PARENT(elm) = (node);                    \
    RB_AUGMENT(node);                    \
    if ((RB_PARENT(node)))                    \
        RB_AUGMENT(RB_PARENT(node));            \
} while (0)


static void rb_remove_color(struct rb_root *head, struct rb_node *parent,
                struct rb_node *elm)
{
    struct rb_node *pnode;

    while ((elm == NULL || RB_COLOR(elm) == RB_BLACK) &&
        elm != RB_HEAD(head)) {
        if (parent == NULL) {
            return;
        }
        if (RB_LEFT(parent) != elm) {
            pnode = RB_LEFT(parent);
            if (RB_COLOR(pnode) == RB_RED) {
                RB_SET_BLACKRED(pnode, parent);
                RB_ROTATE_RIGHT(head, parent, pnode);/*lint !e548*/
                pnode = RB_LEFT(parent);
            }
            if ((RB_LEFT(pnode) == NULL ||
                RB_COLOR(RB_LEFT(pnode)) == RB_BLACK) &&
                (RB_RIGHT(pnode) == NULL ||
                RB_COLOR(RB_RIGHT(pnode)) == RB_BLACK)) {
                RB_COLOR(pnode) = RB_RED;
                elm = parent;
                parent = RB_PARENT(elm);
            } else {
                if (RB_LEFT(pnode) == NULL ||
                    RB_COLOR(RB_LEFT(pnode)) == RB_BLACK) {
                    struct rb_node *oright;
                    if ((oright = RB_RIGHT(pnode)))
                        RB_COLOR(oright) = RB_BLACK;
                    RB_COLOR(pnode) = RB_RED;
                    RB_ROTATE_LEFT(head, pnode, oright);/*lint !e548*/
                    pnode = RB_LEFT(parent);
                }
                RB_COLOR(pnode) = RB_COLOR(parent);
                RB_COLOR(parent) = RB_BLACK;
                if (RB_LEFT(pnode))
                    RB_COLOR(RB_LEFT(pnode)) = RB_BLACK;
                RB_ROTATE_RIGHT(head, parent, pnode);/*lint !e548*/
                elm = RB_HEAD(head);
                break;
            }
        } else {
            pnode = RB_RIGHT(parent);
            if (RB_COLOR(pnode) == RB_RED) {
                RB_SET_BLACKRED(pnode, parent);
                RB_ROTATE_LEFT(head, parent, pnode);/*lint !e548*/
                pnode = RB_RIGHT(parent);
            }
            if ((RB_LEFT(pnode) == NULL ||
                RB_COLOR(RB_LEFT(pnode)) == RB_BLACK) &&
                (RB_RIGHT(pnode) == NULL ||
                RB_COLOR(RB_RIGHT(pnode)) == RB_BLACK)) {
                RB_COLOR(pnode) = RB_RED;
                elm = parent;
                parent = RB_PARENT(elm);
            } else {
                if (RB_RIGHT(pnode) == NULL ||
                    RB_COLOR(RB_RIGHT(pnode)) == RB_BLACK) {
                    struct rb_node *oleft;
                    if ((oleft = RB_LEFT(pnode)))
                        RB_COLOR(oleft) = RB_BLACK;
                    RB_COLOR(pnode) = RB_RED;
                    RB_ROTATE_RIGHT(head, pnode, oleft);/*lint !e548*/
                    pnode = RB_RIGHT(parent);
                }
                RB_COLOR(pnode) = RB_COLOR(parent);
                RB_COLOR(parent) = RB_BLACK;
                if (RB_RIGHT(pnode))
                    RB_COLOR(RB_RIGHT(pnode)) = RB_BLACK;
                RB_ROTATE_LEFT(head, parent, pnode);/*lint !e548*/
                elm = RB_HEAD(head);
                break;
            }
        }
    }
    if (elm)
        RB_COLOR(elm) = RB_BLACK;
}


void rb_insert_color(struct rb_node *elm, struct rb_root *head)
{
    struct rb_node *parent, *gparent, *pnode;

    if ((NULL == elm) || (NULL == head))
    {
        return;
    }

    while ((parent = RB_PARENT(elm)) &&
        RB_COLOR(parent) == RB_RED) {
        gparent = RB_PARENT(parent);
        if (parent != RB_LEFT(gparent)) {
            pnode = RB_LEFT(gparent);
            if (pnode && RB_COLOR(pnode) == RB_RED) {
                RB_COLOR(pnode) = RB_BLACK;
                RB_SET_BLACKRED(parent, gparent);
                elm = gparent;
                continue;
            }
            if (RB_LEFT(parent) == elm) {
                RB_ROTATE_RIGHT(head, parent, pnode);/*lint !e548*/
                pnode = parent;
                parent = elm;
                elm = pnode;
            }
            RB_SET_BLACKRED(parent, gparent);
            RB_ROTATE_LEFT(head, gparent, pnode);/*lint !e548*/
        } else {
            pnode = RB_RIGHT(gparent);
            if (pnode && RB_COLOR(pnode) == RB_RED) {
                RB_COLOR(pnode) = RB_BLACK;
                RB_SET_BLACKRED(parent, gparent);
                elm = gparent;
                continue;
            }
            if (RB_RIGHT(parent) == elm) {
                RB_ROTATE_LEFT(head, parent, pnode);/*lint !e548*/
                pnode = parent;
                parent = elm;
                elm = pnode;
            }
            RB_SET_BLACKRED(parent, gparent);
            RB_ROTATE_RIGHT(head, gparent, pnode);/*lint !e548*/
        }
    }
    RB_COLOR(head->rb_node) = RB_BLACK;
}


void rb_replace_node(struct rb_node *victim, struct rb_node *newnode,
                     struct rb_root *root)
{
        struct rb_node *pnode = NULL;
        if ((NULL == victim) || (NULL == newnode) || (NULL == root))
        {
            return;
        }

        pnode = victim->rb_parent;

        /* Set the surrounding nodes to point to the replacement */
        if (!pnode) {
                root->rb_node = newnode;
        }
        else {
                if (victim != pnode->rb_left)
                        pnode->rb_right = newnode;
                else
                        pnode->rb_left = newnode;
        }

        if (victim->rb_right)
                victim->rb_right->rb_parent = newnode;
        if (victim->rb_left)
                victim->rb_left->rb_parent = newnode;

        /* Copy the pointers/colour from the victim to the replacement */
        *newnode = *victim;
}

struct rb_node *rb_first(struct rb_root *root)
{
        struct rb_node  *pnode = NULL;
        if (NULL == root)
            return NULL;

        pnode = root->rb_node;
        if (!pnode)
                return 0;
        while (pnode->rb_left)
                pnode = pnode->rb_left;
        return pnode;
}


struct rb_node *rb_prev(struct rb_node *elm)
{
    if (NULL == elm)
        return NULL;

    if (RB_LEFT(elm)) {
        elm = RB_LEFT(elm);
        while (RB_RIGHT(elm))
            elm = RB_RIGHT(elm);
    } else {
        if (RB_PARENT(elm) &&
            (elm == RB_RIGHT(RB_PARENT(elm))))
            elm = RB_PARENT(elm);
        else {
            while (RB_PARENT(elm) &&
                (elm == RB_LEFT(RB_PARENT(elm))))
                elm = RB_PARENT(elm);
            elm = RB_PARENT(elm);
        }
    }
    return (elm);
}


struct rb_node *rb_next(struct rb_node *elm)
{
    if(NULL == elm)
        return NULL;

    if (RB_RIGHT(elm)) {
        elm = RB_RIGHT(elm);
        while (RB_LEFT(elm))
            elm = RB_LEFT(elm);
    } else {
        if (RB_PARENT(elm) &&
            (elm == RB_LEFT(RB_PARENT(elm))))
            elm = RB_PARENT(elm);
        else {
            while (RB_PARENT(elm) &&
                (elm == RB_RIGHT(RB_PARENT(elm))))
                elm = RB_PARENT(elm);
            elm = RB_PARENT(elm);
        }
    }
    return (elm);
}


/* Note name changed. Guess why :) */
void rb_erase(struct rb_node *elm, struct rb_root *head)
{
    struct rb_node *child, *parent, *pnode = elm;
    int color;
    if(NULL == elm || NULL == head)
        return;

    if (RB_RIGHT(elm) == NULL)
        child = RB_LEFT(elm);
    else if (RB_LEFT(elm) == NULL)
        child = RB_RIGHT(elm);
    else {
        struct rb_node *left;
        elm = RB_RIGHT(elm);
        while ((left = RB_LEFT(elm)))
            elm = left;
        child = RB_RIGHT(elm);
        parent = RB_PARENT(elm);
        color = RB_COLOR(elm);
        if (child)
            RB_PARENT(child) = parent;
        if (!parent)
            RB_HEAD(head) = child;
        else {
            if (RB_LEFT(parent) == elm)
                RB_LEFT(parent) = child;
            else
                RB_RIGHT(parent) = child;
            RB_AUGMENT(parent);
        }

        if (RB_PARENT(elm) == pnode)
            parent = elm;
        *(elm) = *(pnode);
        if (!RB_PARENT(pnode))
            RB_HEAD(head) = elm;
        else {
            if (RB_LEFT(RB_PARENT(pnode)) == pnode)
                RB_LEFT(RB_PARENT(pnode)) = elm;
            else
                RB_RIGHT(RB_PARENT(pnode)) = elm;
            RB_AUGMENT(RB_PARENT(pnode));
        }

        RB_PARENT(RB_LEFT(pnode)) = elm;
        if (RB_RIGHT(pnode))
            RB_PARENT(RB_RIGHT(pnode)) = elm;
        if (parent) {
            left = parent;
            do {
                RB_AUGMENT(left);
            } while ((left = RB_PARENT(left)));
        }
        goto color;
    }
    parent = RB_PARENT(elm);
    color = RB_COLOR(elm);
    if (child)
        RB_PARENT(child) = parent;
    if (!parent)
        RB_HEAD(head) = child;
    else {
        if (RB_LEFT(parent) == elm)
            RB_LEFT(parent) = child;
        else
            RB_RIGHT(parent) = child;
        RB_AUGMENT(parent);
    }

color:
    if (color == RB_BLACK)
        rb_remove_color(head, parent, child);
}
