BUVER="2.35"
GCCVER="10.2.0"
PREFIX="$PWD/cross"
SH_HOME="$PWD"

cd $SH_HOME

if [ -d "cross" ]; then
	echo "Compiled binaries are already exists."
	exit 0
fi

# Configuring packages for building

if [ "$OSTYPE" == "linux-gnu" ]; then
	REQUIRED_PKG="wget build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo nasm mtools"
	CC="gcc"
	MAKE="make"
	apt-get --yes install $REQUIRED_PKG
elif [ "$OSTYPE" == "FreeBSD" ]; then
	REQUIRED_PKG="wget binutils bzip2 gcc gmake bison flex gmp mpc mpfr texinfo nasm mtools"
	CC="gcc"
	MAKE="gmake"
	pkg install -y $REQUIRED_PKG
else
	echo "Unknown OS Type";
	exit 1
fi

# Downloading src files for building cross-compiler

echo "Receiving archive files : "
if ! [ -f "binutils-$BUVER.tar.gz" ] ; then
	wget "ftp://ftp.gnu.org/gnu/binutils/binutils-$BUVER.tar.gz"
fi
if ! [ -f "gcc-$GCCVER.tar.gz" ] ; then
	wget "ftp://ftp.gnu.org/gnu/gcc/gcc-$GCCVER/gcc-$GCCVER.tar.gz"
fi

echo "Unpacking archive files :"
if ! [ -d "binutils-$BUVER" ] ; then
	tar -xvf "binutils-$BUVER.tar.gz"
fi
if ! [ -d "gcc-$GCCVER" ] ; then
	tar -xvf "gcc-$GCCVER.tar.gz"
fi

mv "binutils-$BUVER" "binutils"
mv "gcc-$GCCVER" "gcc"


# Building binutils i686-elf

echo "Building binutils for target system i686-elf : "

rm -rf binutils-build
mkdir binutils-build
cd binutils-build
../binutils/configure --target=i686-elf --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

$MAKE
$MAKE install
cd $SH_HOME

# Building binutils for x86_64-elf

echo "Building binutils for target system x86_64-elf : "

rm -rf binutils-build
mkdir binutils-build
cd binutils-build
../binutils/configure --target=x86_64-elf --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

$MAKE
$MAKE install
cd $SH_HOME

# Building GCC i686-elf

echo "Building GCC for target system i686-elf : "

rm -rf gcc-build
mkdir gcc-build
cd gcc-build
../gcc/configure --target=i686-elf --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

$MAKE all-gcc
$MAKE all-target-libgcc
$MAKE install-gcc
$MAKE install-target-libgcc
cd $SH_HOME

# Building GCC x86_64-elf

echo "Building GCC for target system x86_64-elf : "

rm -rf gcc-build
mkdir gcc-build
cd gcc-build
../gcc/configure --target=x86_64-elf --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

$MAKE all-gcc
$MAKE all-target-libgcc
$MAKE install-gcc
$MAKE install-target-libgcc
cd $SH_HOME

# Cleaning Files

rm -rf binutils
rm -rf gcc
rm -rf binutils-build
rm -rf gcc-build
rm -rf "binutils-$BUVER.tar.gz"
rm -rf "gcc-$GCCVER.tar.gz"
