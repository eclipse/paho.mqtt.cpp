Summary:            MQTT CPP Client
Name:               paho-cpp
Version:            1.2.0
Release:            1%{?dist}
License:            Eclipse Distribution License 1.0 and Eclipse Public License 1.0
Group:              Development/Tools
Source:             https://github.com/eclipse/paho.mqtt.cpp/archive/refs/tags/v-%{version}.tar.gz#/paho.mqtt.cpp-%{version}.tar.gz
URL:                https://eclipse.org/paho/clients/cpp/
BuildRequires:      cmake3
BuildRequires:      gcc
BuildRequires:      graphviz
BuildRequires:      doxygen
BuildRequires:      openssl-devel
BuildRequires:      paho-c-devel
Requires:           openssl
Requires:           paho-c >= 1.3.8


%description
The Paho MQTT CPP Client is a fully fledged MQTT client written in ANSI standard C++ 11.


%package devel
Summary:            MQTT CPP Client development kit
Group:              Development/Libraries
Requires:           paho-cpp

%description devel
Development files and samples for the the Paho MQTT CPP Client.


%package devel-docs
Summary:            MQTT CPP Client development kit documentation
Group:              Development/Libraries

%description devel-docs
Development documentation files for the the Paho MQTT CPP Client.

%prep
%autosetup -n paho.mqtt.cpp-%{version}

%build
mkdir build.paho.cpp && cd build.paho.cpp
%cmake -DPAHO_WITH_SSL=TRUE -DPAHO_BUILD_DOCUMENTATION=TRUE ..
%cmake_build

%install
pushd build.paho.cpp
%cmake_install
popd

mkdir -p %{buildroot}%{_datadir}/doc/%{name}/samples/
# put the samples into the documentation directory
cp -a src/samples/*.cpp %{buildroot}%{_datadir}/doc/%{name}/samples/
# Put paho html docs in a paho subdirectory
mv %{buildroot}%{_datadir}/doc/html %{buildroot}%{_datadir}/doc/%{name}

%files
%doc edl-v10 epl-v10
%{_libdir}/*

%files devel
%{_includedir}/*
/usr/lib/cmake/PahoMqttCpp
%doc %{_docdir}/%{name}/samples/
%doc %{_docdir}/%{name}/html/

%changelog
* Fri Feb 26 2021 Joshua Clayton <joshua.clayton@3deolidar.com> - 1.2.0
- Update for version 1.2.0
* Tue Dec 08 2020 Joshua Clayton <joshua.clayton@3deolidar.com> - 1.1
- Update and patch for 1.1
- Put the html documenation into an appropriate paho-cpp directory
- instead of compiling the samples, put the cpp files in with documentation
* Wed Oct 11 2017 Julien Courtat <julien.courtat@aqsacom.com> - 1.0.0
- Initial packaging
