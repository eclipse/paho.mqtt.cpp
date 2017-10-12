VERSION=1.0.0

check:
	rpmlint -i dist/paho-cpp.spec

rpm-prep:
	mkdir -p ${HOME}/rpmbuild/SOURCES/
	tar --transform="s/\./paho-cpp-${VERSION}/" -cf ${HOME}/rpmbuild/SOURCES/v${VERSION}.tar.gz --exclude=./build.paho.cpp --exclude=.git --exclude=*.bz ./ --gzip

rpm: rpm-prep
	rpmbuild -ba dist/paho-cpp.spec
