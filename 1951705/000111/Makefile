dllsrc=readconfig.c
dllname=_1951705
dllfilename=lib$(dllname).so

target=test
src=$(target).c
obj=$(target).o
dllFlag=-shared

CC=gcc

dll_path=$(DESTDIR)/usr/lib64/stu
install_target=test_1951705
install_path=$(DESTDIR)/usr/sbin/stu

dat_path=$(DESTDIR)/home/stu/u1951705
dat_filename=1951705.dat

conf_path=$(DESTDIR)/etc/stu
conf_filename=stu_1951705.conf

service_path=$(DESTDIR)/usr/lib/systemd/system
service_filename=test_1951705.service

version=1.0
rpmbuild_path=~/rpmbuild
rpm_filename=test_1951705.rpm
rpmSpec_filename=test_1951705.spec
rpmSpec_path=$(rpmbuild_path)/SPECS
rpmSource_path=$(rpmbuild_path)/SOURCES
srcdir = $(target)_1951705-$(version)
srctar = $(target)_1951705.tar.gz
etc_link_service_path=$(DESTDIR)/etc/systemd/system


all: so $(target)

$(target):$(obj)
	$(CC) $(obj) -o $(target) -Wl,-rpath="$(dll_path)" -l$(dllname) -L $(dll_path) 

%.o:%.cpp
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm -f $(dllfilename)
	rm -f $(obj)
	rm -f $(rpm_filename)
	rm -f $(target)
	rm -f $(srctar)
	rm -rf $(srcdir)

.PHONY:so
so:
	gcc $(dllFlag) -fPIC -o $(dllfilename) $(dllsrc);
	mkdir -p $(dll_path)
	cp -f $(dllfilename) $(dll_path)/$(dllfilename)
# ¼Ó\Ç¿ÖÆ¸²¸Ç

.PHONY:install
install:
	mkdir -p $(install_path)
	cp -f $(target) $(install_path)/$(install_target)
	mkdir -p $(dll_path)
	cp -f $(dllfilename) $(dll_path)/$(dllfilename)
	mkdir -p $(dat_path)
	cp -f $(dat_filename) $(dat_path)/$(dat_filename)
	mkdir -p $(conf_path)
	cp -f $(conf_filename) $(conf_path)/$(conf_filename)
	mkdir -p $(service_path)
	cp -f $(service_filename) $(service_path)/$(service_filename)
	systemctl daemon-reload


.PHONY:uninstall
uninstall:
	@if [ "$(shell ls $(dat_path))" = $(dat_filename) ];then \
		rm -rf $(dat_path);\
		echo "delete dir";\
	else \
		rm -f $(dat_path)/$(dat_filename);\
	fi
	rm -f $(conf_path)/$(conf_filename)
	rm -f $(dll_path)/$(dllfilename)
	rm -f $(service_path)/$(service_filename)
	rm -f $(etc_link_service_path)/$(service_filename)
	rm -rf $(rpmbuild_path)
	rm -f $(install_path)/$(install_target)


.PHONY:rpm
rpm:
	mkdir -p $(srcdir)
	cp -f $(dllsrc) $(src) $(dat_filename) $(conf_filename) $(service_filename) Makefile $(srcdir)
	tar -cz $(srcdir) -f $(srctar)
	mkdir -p $(rpmbuild_path)/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
	cp -f $(srctar) $(rpmSource_path)/$(srctar)
	cp -f $(rpmSpec_filename) $(rpmSpec_path)/$(rpmSpec_filename)
	rpmbuild -ba $(rpmSpec_path)/$(rpmSpec_filename)
	cp -f $(rpmbuild_path)/RPMS/x86_64/test_1951705* ./$(rpm_filename)
