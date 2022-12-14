Name: resiprocate
Version: 1.9.0
Release: 1
Summary: Resiprocate SIP Stack
License: Vovida Software License http://opensource.org/licenses/vovidapl.php
Group: Productivity/Telephony/SIP/Servers
Vendor: resiprocate.org
Packager: Daniel Pocock <daniel@pocock.com.au>
Url: http://www.resiprocate.org
Source: %name-%version.tar.gz
BuildRequires: libtool automake autoconf
BuildRequires: asio-devel
BuildRequires: boost-devel
BuildRequires: c-ares-devel
BuildRequires: cppunit-devel
BuildRequires: gperf
BuildRequires: db4-devel
BuildRequires: openssl-devel >= 0.9.8
BuildRequires: mysql-devel
BuildRequires: pcre-devel
BuildRequires: perl
BuildRequires: popt-devel
BuildRequires: radiusclient-ng-devel
BuildRequires: xerces-c-devel
Requires: openssl >= 0.9.8
Requires: chkconfig
Prefix: %_prefix
BuildRoot: %{_tmppath}/%name-%version-root

%description
The reSIProcate components, particularly the SIP stack, are in use in both
commercial and open-source products. The project is dedicated to maintaining
a complete, correct, and commercially usable implementation of SIP and a few
related protocols.

%package -n libresiprocate-1_8
Summary: shared libraries http://www.resiprocate.org
Group: Productivity/Telephony/SIP

%description -n libresiprocate-1_8
reSIProcate is a framework that aims to fully implement the SIP protocol
in first class C++.  It is intended for use in other applications,
such as the repro SIP proxy.
This package provides the core libraries: librutil (utilities),
libresip (SIP stack/RFC compliant message parsing) and
libdum (SIP Dialog Usage Manager, a state machine for SIP dialogs).

%package 1_8-devel
Summary: Resiprocate development files
Group: Development/Libraries
Requires: %{name} = %{version}

%description 1_8-devel
Resiprocate SIP Stack development files.

%package repro
Summary: SIP proxy
Group: Productivity/Telephony/SIP/Servers
Requires(pre): shadow-utils

%description repro
repro is the SIP proxy developed as part of the reSIProcate project.
It provides a high-quality, low maintenance solution for serving
small and large IP telephony installations.
reSIProcate is a framework that aims to fully implement the SIP protocol
in first class C++.  It is intended for use in other applications,
such as the repro SIP proxy.

%package turn-server
Summary: ICE/STUN/TURN server
Group: Productivity/Telephony/SIP/Servers
Requires(pre): shadow-utils

%description turn-server
reTurn is the TURN server developed as part of the reSIProcate project.
TURN (RFC 5766) provides a standardised solution for VoIP applications
to find the most efficient way to route media streams when NAT and
firewall devices may be present.
reTurn provides a high-quality, low maintenance solution for serving
small and large IP telephony installations.  It has been used successfully
with a variety of SIP and Jabber applications, including Lumicall,
Jitsi, Empathy and Psi.
reSIProcate is a framework that aims to fully implement the SIP protocol
in first class C++.  It is intended for use in other applications,
such as the repro SIP proxy.

%package sipdialer
Summary: click-to-call utility
Group: Productivity/Telephony/SIP/Servers

%description sipdialer
sipdialer is a click-to-call application that uses the SIP REFER
method to force a SIP phone (for example, the phone on your desk)
to dial another number.  It can be registered in GConf to handle
clicks on sip:, sips: and tel: URIs in other applications and the
web browser.
reSIProcate is a framework that aims to fully implement the SIP protocol
in first class C++.  It is intended for use in other applications,
such as the repro SIP proxy.

%package b2bua
Summary: basic SIP B2BUA
Group: Productivity/Telephony/SIP/Servers

%description b2bua
Provides a simple SIP B2BUA configured from a static XML file.
reSIProcate is a framework that aims to fully implement the SIP protocol
in first class C++.  It is intended for use in other applications,
such as the repro SIP proxy.

%package clicktocall
Summary: click-to-call server process
Group: Productivity/Telephony/SIP/Servers

%description clicktocall
Provides a simple SIP click-to-call service.
reSIProcate is a framework that aims to fully implement the SIP protocol
in first class C++.  It is intended for use in other applications,
such as the repro SIP proxy.

%package presence-server
Summary: basic SIP presence server
Group: Productivity/Telephony/SIP/Servers

%description presence-server
Provides a simple SIP presence server.
reSIProcate is a framework that aims to fully implement the SIP protocol
in first class C++.  It is intended for use in other applications,
such as the repro SIP proxy.

%prep
%setup -q

%build
%configure LDFLAGS=-L%{_libdir}/mysql --with-c-ares --with-ssl --with-mysql --with-radius --with-b2bua --with-apps --enable-ipv6
make

%install
make DESTDIR=%buildroot install
install -p -D -m644 repro/pkg/fedora/repro.service $RPM_BUILD_ROOT%{_unitdir}/repro.service
install -p -D -m644 reTurn/pkg/fedora/resiprocate-turn-server.service $RPM_BUILD_ROOT%{_unitdir}/resiprocate-turn-server.service
install -m 0755 -d $RPM_BUILD_ROOT/etc/repro
cat repro/repro.config | \
  sed -e 's!^DatabasePath = ./$!DatabasePath = /var/lib/repro!' \
    > $RPM_BUILD_ROOT/etc/repro/repro.config
chmod 0644 $RPM_BUILD_ROOT/etc/repro/repro.config
install -p -D -m644 reTurn/reTurnServer.config $RPM_BUILD_ROOT/etc/reTurnServer.config
install -m 0755 -d $RPM_BUILD_ROOT/var/lib/repro


%clean
[ ${RPM_BUILD_ROOT} != "/" ] && rm -rf ${RPM_BUILD_ROOT}

%pre repro
getent group repro >/dev/null || groupadd -r repro
getent passwd repro >/dev/null || \
    useradd -r -g repro -d /var/lib/repro -s /sbin/nologin \
    -c "repro SIP proxy" repro

%pre turn-server
getent group return >/dev/null || groupadd -r return
getent passwd return >/dev/null || \
    useradd -r -g return -d / -s /sbin/nologin \
    -c "reTurn TURN server" return

%post -n libresiprocate-1_8 -p /sbin/ldconfig

%post repro
/sbin/ldconfig
if [ $1 -eq 1 ] ; then 
    # Initial installation
    /bin/systemctl enable repro.service >/dev/null 2>&1 || :
fi
/bin/chmod 0755 /var/lib/repro

%post turn-server
if [ $1 -eq 1 ] ; then
    # Initial installation
    /bin/systemctl enable resiprocate-turn-server.service >/dev/null 2>&1 || :
fi

%preun repro
if [ "$1" = 0 ]
then
   # Package removal, not upgrade
   /bin/systemctl --no-reload disable repro.service > /dev/null 2>&1 || :
   /bin/systemctl stop repro.service > /dev/null 2>&1 || :
fi

%preun turn-server
if [ "$1" = 0 ]
then
   # Package removal, not upgrade
   /bin/systemctl --no-reload disable resiprocate-turn-server.service > /dev/null 2>&1 || :
   /bin/systemctl stop resiprocate-turn-server.service > /dev/null 2>&1 || :
fi

%postun -n libresiprocate-1_8 -p /sbin/ldconfig
%postun repro
/sbin/ldconfig
/bin/systemctl daemon-reload >/dev/null 2>&1 || :
if [ $1 -ge 1 ] ; then
    # Package upgrade, not uninstall
    /bin/systemctl try-restart repro.service >/dev/null 2>&1 || :
fi

%postun turn-server
/bin/systemctl daemon-reload >/dev/null 2>&1 || :
if [ $1 -ge 1 ] ; then
    # Package upgrade, not uninstall
    /bin/systemctl try-restart resiprocate-turn-server.service >/dev/null 2>&1 || :
fi

%files -n libresiprocate-1_8
%defattr(644,root,root,755)
%{_libdir}/librutil*.so
%{_libdir}/libresip*.so
%{_libdir}/libdum*.so
%{_libdir}/libb2bua*.so

%files 1_8-devel
%defattr(644,root,root,755)
%{_includedir}/rutil/*.hxx
%{_includedir}/rutil/*/*.hxx
%{_includedir}/resip/stack/*.hxx
%{_includedir}/resip/stack/ssl/*.hxx
%{_includedir}/resip/dum/*.hxx
%{_includedir}/resip/dum/ssl/*.hxx
%{_includedir}/repro/*.hxx
%{_includedir}/repro/*/*.hxx
%{_includedir}/b2bua/*.hxx
%{_includedir}/clicktocall/*.hxx
%{_includedir}/reTurnClient/*.hxx
%{_includedir}/sipdial/*.hxx
%{_libdir}/lib*.a
%{_libdir}/lib*.la

%files repro
%defattr(644,root,root,755)
%attr(0755,repro,repro) %dir /var/lib/repro
%config(noreplace) /etc/repro/repro.config
%{_unitdir}/repro.service
%{_libdir}/librepro*.so
%attr(755,-,-) %{_sbindir}/repro
%attr(755,-,-) %{_sbindir}/reprocmd
%{_mandir}/man8/repro*.8.gz

%files turn-server
%defattr(644,root,root,755)
%config(noreplace) /etc/reTurnServer.config
%{_unitdir}/resiprocate-turn-server.service
%attr(755,-,-) %{_sbindir}/reTurnServer
%{_libdir}/libreTurnClient*.so
%{_mandir}/man8/reTurnServer.8.gz

%files sipdialer
%defattr(644,root,root,755)
%{_libdir}/libsipdial*.so
%attr(755,-,-) %{_bindir}/sipdialer
%{_mandir}/man1/sipdialer.1.gz

%files clicktocall
%defattr(644,root,root,755)
%attr(755,-,-) %{_sbindir}/clicktocall

%files b2bua
%defattr(644,root,root,755)
%attr(755,-,-) %{_bindir}/basicB2BUA

%files presence-server
%defattr(644,root,root,755)
%attr(755,-,-) %{_sbindir}/presSvr

%changelog
* Sat Nov 24 2012 Daniel Pocock <daniel@pocock.com.au> -- Produce multiple packages for stack/libs, daemons, sipdialer
* Sun Aug  7 2011 Daniel Pocock <daniel@pocock.com.au> -- Initial build based on autotools

