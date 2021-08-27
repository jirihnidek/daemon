Name:           daemon
Version:        0.1
Release:        1%{?dist}
Summary:        Example of daemon

License:        GPL
URL:            https://github.com/jirihnidek/daemon
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  cmake


%description
This package contains example of simple UNIX daemon


# Section for preparation of build
%prep

# Following macro just has to be here. It unpacks the original source from
# tag.gz archive. It is "interesting" that rpmbuild does not do this
# automatically, when Source0 is defined, but you have to call it explicitly.
%setup -q


# Build section
%build

# We have to use build type "Debug" to be able to create all variants of
# rpm packages (debuginfo, debugsource). The normal rpm is stripped from
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
# that are part of final RPM package.
%files

%attr(755,root,root) %{_bindir}/daemon
%attr(755,root,root) %dir %{_sysconfdir}/daemon
%attr(750,root,root) %{_sysconfdir}/daemon/daemon.conf
%attr(644,root,root) %{_unitdir}/simple-daemon.service
%attr(644,root,root) %{_unitdir}/forking-daemon.service


# This is section, where you should describe all important changes
# in RPM
%changelog

* Fri Aug 27 2021 Jiri Hnidek <jhnidek@redhat.com>
- new package built with tito

* Fri Aug 27 2021 Jiri Hnidek <jhnidek@redhat.com>
- Added first version of daemon.spec
