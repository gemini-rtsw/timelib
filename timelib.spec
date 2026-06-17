%define _prefix /gem_base/epics/support
%define name timelib
%define repository gemdev
%define debug_package %{nil}
%define arch %(uname -m)
%define checkout %(if [ -n "$GIT_HASH" ]; then echo "$GIT_HASH"; else git rev-parse --short HEAD 2>/dev/null || echo nogit; fi)

#These global defines are added to prevent stripping
# symbols on vxWorks cross-compiled code
# Getting 'strip' to work is probably only needed for
# building a related debug sub-package
#
# But this prevents all the strip warnings
# mrippa 20120202
%global _enable_debug_package 0
%global debug_package %{nil}
%global __os_install_post /usr/lib/rpm/brp-compress %{nil}

Summary: %{name} Package, a module for EPICS base
Name: %{name}
Version: 2.1.4
Release: 3.git.%{checkout}%{?dist}
License: EPICS Open License
Group: Applications/Engineering
Source0: %{name}-%{version}.tar.gz
ExclusiveArch: %{arch}
Prefix: %{_prefix}
## You may specify dependencies here
BuildRequires: epics-base-devel = 7.0.7-0.git.1159d86%{?dist} re2c gemini-ade slalib-devel sequencer-devel
## (runtime Requires removed: cross-compiled VME/build-only artifact, never runs on host)
## Switch dependency checking off
# AutoReqProv: no

%description
This is the module %{name}.

## If you want to have a devel-package to be generated uncomment the following:
%package devel
Summary: %{name}-devel Package
Group: Development/Gemini
Requires: %{name}
%description devel
This is the module %{name}.

%prep
%setup -q 

%build
make distclean uninstall
make

%install
export DONT_STRIP=1
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_prefix}/%{name}
cp -r dbd $RPM_BUILD_ROOT/%{_prefix}/%{name}
#cp -r bin $RPM_BUILD_ROOT/%{_prefix}/%{name}
cp -r lib $RPM_BUILD_ROOT/%{_prefix}/%{name}
cp -r include $RPM_BUILD_ROOT/%{_prefix}/%{name}
cp -r configure $RPM_BUILD_ROOT/%{_prefix}/%{name}
# find $RPM_BUILD_ROOT/%{_prefix}/%{name}/configure -name ".git" -exec rm -rf {} \;


%postun
if [ "$1" = "0" ]; then
	rm -rf %{_prefix}/%{name}
fi


%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
#   /%{_prefix}/%{name}/bin
   /%{_prefix}/%{name}/lib

%files devel
%defattr(-,root,root)
   /%{_prefix}/%{name}/dbd
   /%{_prefix}/%{name}/include
   /%{_prefix}/%{name}/configure

%changelog
* Mon Jan 11 2021 Roberto Rojas <rrojas@gemini.edu> 2.1.4-3
- Changes to fix the LST issue

* Thu Oct 08 2020 fkraemer <fkraemer@gemini.edu> 2.1.4-2
- applied new version/release scheme and new yum repository structure
- Automatic commit of package [timelib] release
  [3.15.8-2.1.4.2020080504548d6ed0c].

* Thu Aug 27 2020 Felix Kraemer <fkraemer@gemini.edu> 3.15.8-2.1.4.20200827111187ee95b
- Deleted WORK_SUPP definition in configure/RELEASE because it causes conflicts
  if used in other modules For local development, add a configure/RELEASE.local
  file to overwrite definitions in configure/RELEASE Added
  configure/RELEASE.local to .gitignore file (fkraemer@gemini.edu)
- Added support dbd file to be used by IOCs (iarriagada@gemini.edu)

* Wed Aug 05 2020 fkraemer <fkraemer@gemini.edu> 3.15.8-2.1.4.2020080504548d6ed0c
- Release tag enriched with hour and minute (%%H%%M) to be able to build
  several RPMs a day without messing up the repo (fkraemer@gemini.edu)
- fixed support module path (fkraemer@gemini.edu)

* Sun Jul 26 2020 fkraemer <fkraemer@gemini.edu> 3.15.8-2.1.4.20200726abab08c
- new package built with tito

