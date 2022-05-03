%define defaultbuildroot /
AutoProv: no
%undefine __find_provides
AutoReq: no
%undefine __find_requires
%define __spec_install_post %{nil}
%define debug_package %{nil}
%define _home /home
%undefine __check_files

Name:    test_1951705
Version: 1.0
Release: soft%{?dist}
Summary: test_1951705
BuildArch: x86_64

License: BSD
Source: %{name}.tar.gz

Prefix:        %{_prefix}
Requires(post): /bin/sh
Requires(postun): /bin/sh

%description
test_1951705

%prep
%setup -q

%build
make

%install
%make_install

%files
%{_sysconfdir}/stu/stu_1951705.conf
%{_prefix}/lib64/stu/lib_1951705.so
%{_sbindir}/stu/test_1951705
%{_home}/stu/u1951705/1951705.dat
%{_home}/stu/u1951705
%{_prefix}/lib/systemd/system/test_1951705.service

%pre
echo "prepare to install %{name}"

%post
echo "finish installing %{name}"
ln -s %{_prefix}/lib/systemd/system/test_1951705.service %{_sysconfdir}/systemd/system/test_1951705.service
systemctl daemon-reload

%preun
echo "prepare to uninstall %{name}"

%postun
echo "finish uninstalling %{name}"
systemctl daemon-reload

%clean
echo "echo clean"
%changelog
