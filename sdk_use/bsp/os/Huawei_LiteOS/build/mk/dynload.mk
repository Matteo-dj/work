
dynload:
	cp tools/fsimage/rootfs_mn34220_30fps_2k_4bit.yaffs2 $(OUT)/rootfs_mn34220_30fps_2k_4bit.yaffs2
	cp tools/fsimage/rootfs_64k.jffs2 $(OUT)/rootfs_64k.jffs2

dynload_ld:
#	mkdir -p $(OUT);
	cd $(LITEOSTOPDIR)/test/sample/dynload/elfs_explicit; make clean; make;
	cp $(LITEOSTOPDIR)/test/sample/dynload/elfs_explicit/*.so $(LITEOSTOPDIR)/tools/elfs/
	cp $(LITEOSTOPDIR)/test/sample/dynload/elfs_explicit/*.o $(LITEOSTOPDIR)/tools/elfs/

	cd $(LITEOSTOPDIR)/test/sample/dynload/elfs_implicit; make clean; make;
	cp $(LITEOSTOPDIR)/test/sample/dynload/elfs_implicit/*.so $(LITEOSTOPDIR)/tools/elfs/

	cd $(LITEOSTOPDIR)/tools/elfs; make; cd -;
	cd $(SCRIPTS_PATH)/dynload_tools; ./sym.sh $(LITEOSTOPDIR)/tools/elfs

	mkdir -p $(LITEOSTOPDIR)/tools/fsimage/rootfs/bin/dynload
	cp $(LITEOSTOPDIR)/tools/elfs/*.so $(LITEOSTOPDIR)/tools/fsimage/rootfs/bin/dynload
	cp $(LITEOSTOPDIR)/tools/elfs/*.o $(LITEOSTOPDIR)/tools/fsimage/rootfs/bin/dynload
	cd tools/fsimage && ./MakeVersion.sh && cd -
#	cp tools/fsimage/rootfs_mn34220_30fps_2k_4bit.yaffs2 $(OUT)/rootfs_mn34220_30fps_2k_4bit.yaffs2
#	cp tools/fsimage/rootfs_64k.jffs2 $(OUT)/rootfs_64k.jffs2

dynloadclean:
	cd $(LITEOSTOPDIR)/tools/elfs; make clean; cd -;
	rm -rf $(LITEOSTOPDIR)/build/mk/dynload_ld.mk
	@-$(RM) $(LITEOSTOPDIR)/kernel/extended/dynload/src/los_dynload_gsymbol.c
	@-$(RM) $(LITEOSTOPDIR)/tools/fsimage/rootfs_mn34220_30fps_2k_4bit.yaffs2
	@-$(RM) $(LITEOSTOPDIR)/tools/fsimage/rootfs_64k.jffs2
	@echo "clean dynload image finish"


.PHONY : dynload dynload_ld dynloadclean
