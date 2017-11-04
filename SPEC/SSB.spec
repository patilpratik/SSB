Name:           SSB
Version:    	1.0
Release:        1%{?dist}
Summary:        Secure Storage Box
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

License:        GPLv3+
#URL:            
Source0:        SSB-1.0.0-1.tar.gz 

#BuildRequires:  
#Requires:       

%description
This application is intended to provide secure box for putting up your stuff.

%prep
%autosetup


%build
#%configure
make %{?_smp_mflags}


%install
rm -rf %{buildroot}
mkdir -p %{buildroot}%{_bindir}
make DESTDIR=%{buildroot}%{_bindir} install


%files
%defattr(-,root,root,-)
%attr(0755,gold,fish) %{_bindir}/ssb
%doc



%changelog
* Wed Nov 12 2014 pratik
- 
