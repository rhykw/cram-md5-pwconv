Name:		cram-md5-pwconv
Version:	1.0.0
Release:	1%{?dist}
Summary:	Covert clear password to CRAM-MD5 encrypted password.
License:	MIT
URL:		https://github.com/rhykw/cram-md5-pwconv
Source0:	cram-md5-pwconv-%{version}.tar.gz

BuildRequires:	openssl-devel
Requires:	openssl

%description
Covert clear password to CRAM-MD5 encrypted password.

%prep
%setup -q


%build
make %{?_smp_mflags} prefix=%{_prefix}


%install
%make_install prefix=%{_prefix}


%files
%{_sbindir}/cram-md5-pwconv


%changelog
* Sun Oct  8 2017 rhykw <dev@rhykw.net> - 1.0.0-1
- Release first package.
