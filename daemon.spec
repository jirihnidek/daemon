%global groupname daemoner
%global username  daemoner
%global homedir   /


Name:           daemon
Version:        0.2
Release:        1%{?dist}
Summary:        Example of daemon

License:        GPL
URL:            https://github.com/jirihnidek/daemon
Source0:        %{name}-%{version}.tar.gz

Requires(pre):  shadow-utils

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  cmake
BuildRequires:  systemd
BuildRequires:  systemd-rpm-macros


%description
This package contains example of simple UNIX daemon


%pre
getent group %{groupname} >/dev/null || groupadd -r %{groupname}
getent passwd %{username} >/dev/null || \
    useradd -r -g %{groupname} -d %{homedir} -s /sbin/nologin \
    -c "User used for running example of daemon" %{username}
exit 0


# Section for preparation of build
%prep

# Following macro just has to be here. It unpacks the original source from
# tag.gz archive. It is "interesting" that rpmbuild does not do this
# automatically, when Source0 is defined, but you have to call it explicitly.
%setup -q


# Build section
%build

# We have to use build type "Debug" to be able to create all variants of
# rpm packages (debuginfo, debug source). The normal rpm is stripped from
# debug information. Following macro just run cmake and it generates Makefile
%cmake -DCMAKE_BUILD_TYPE="Debug"

# This macro runs make -f Makefile generated in previous step
%cmake_build


# Install section
%install

# Remove previous build results
rm -rf $RPM_BUILD_ROOT

# This macro runs make -f Makefile install and it installs
# all files to $RPM_BUILD_ROOT
%cmake_install


# This is special section again. You have to list here all files
# that are part of final RPM package. You can specify owner of
# files and permissions to files
%files

# Files and directories owned by root:root
%attr(755,root,root) %{_bindir}/daemon
%attr(755,root,root) %dir %{_sysconfdir}/daemon
%attr(644,root,root) %{_unitdir}/simple-daemon.service
%attr(644,root,root) %{_unitdir}/forking-daemon.service

# File owned by root, but group can read it
%attr(640,root,%{groupname}) %{_sysconfdir}/daemon/daemon.conf

# Files and directories owned by daemoner:daemoner user
%attr(755,%{username},%{groupname}) %{_var}/log/daemon
%attr(755,%{username},%{groupname}) %{_rundir}/daemon



# This is section, where you should describe all important changes
# in RPM
%changelog
* Mon Jan 22 2024 Jiri Hnidek <jhnidek@redhat.com>
- Add initial support for Packit
- Updated version of daemon to 0.2

* Tue Oct 24 2023 Jiri Hnidek <jhnidek@redhat.com>
- Fix building of RPM on COPR

* Wed Sep 29 2021 Jiri Hnidek <jhnidek@redhat.com>
- Use non-root user for runnig daemon

* Mon Sep 27 2021 Jiri Hnidek <jhnidek@redhat.com>
- Use GitHub Actions for building RPM

* Fri Aug 27 2021 Jiri Hnidek <jhnidek@redhat.com>
- RPM package built with tito

* Fri Aug 27 2021 Jiri Hnidek <jhnidek@redhat.com>
- Added first version of daemon.spec
