Summary:            MQTT CPP Client
Name:               paho-cpp
Version:            1.2.0
Release:            1%{?dist}
License:            BSD and EPL-1.0
Source:             https://github.com/eclipse/paho.mqtt.cpp/archive/v%{version}/%{name}-%{version}.tar.gz
URL:                https://eclipse.org/paho/clients/cpp/
BuildRequires:      cmake3
BuildRequires:      gcc-c++
BuildRequires:      graphviz
BuildRequires:      doxygen
BuildRequires:      openssl-devel
BuildRequires:      paho-c-devel
Requires:           openssl
Requires:           paho-c >= 1.3.8


%description
Paho MQTT CPP is a fully fledged MQTT client written in ANSI standard C++ 11.


%package devel
Summary:            MQTT CPP Client development kit
Requires:           %{name}%{?_isa} = %{version}-%{release}

%description devel
Development files for the the Paho MQTT CPP Client.


%package devel-docs
Summary:            MQTT CPP Client development kit documentation
BuildArch:          noarch

%description devel-docs
Development documentation and code samples for the the Paho MQTT CPP Client.


%prep
%autosetup -n paho.mqtt.cpp-%{version}
sed -i'' 's|lib/cmake|%{_lib}/cmake|' cmake/CMakeLists.txt

%build
%cmake -DPAHO_WITH_SSL=TRUE -DPAHO_BUILD_DOCUMENTATION=TRUE
%cmake_build

%install
%cmake_install

mkdir -p %{buildroot}%{_docdir}/%{name}/samples/
# put the samples into the documentation directory
cp -a src/samples/*.cpp %{buildroot}%{_docdir}/%{name}/samples/
# Put paho html docs in a paho subdirectory
mv %{buildroot}%{_docdir}/html %{buildroot}%{_docdir}/%{name}/html

%files
%license edl-v10 epl-v10
%{_libdir}/libpaho-mqttpp3.so.1*

%files devel
%license edl-v10 epl-v10
%{_includedir}/mqtt
%{_libdir}/libpaho-mqttpp3.so
%{_libdir}/cmake/PahoMqttCpp

%files devel-docs
%license edl-v10 epl-v10
%doc CHANGELOG.md CONTRIBUTING.md README.md
%doc %{_docdir}/%{name}

%changelog
* Thu Jun 03 2021 Joshua Clayton <joshua.clayton@3deolidar.com> - 1.2.0
- Update for version 1.2.0
- Fix many details for inclusion in fedora

* Tue Dec 08 2020 Joshua Clayton <joshua.clayton@3deolidar.com> - 1.1
- Update and patch for 1.1
- Put the html documenation into an appropriate paho-cpp directory
- instead of compiling the samples, put the cpp files in with documentation

* Wed Oct 11 2017 Julien Courtat <julien.courtat@aqsacom.com> - 1.0.0
- Initial packaging
