DIR="$( cd "$( dirname "$0" )" && pwd -P )"

mkdir -p build

cmake -B "$DIR/build" -S "$DIR"
ls -l "$DIR/build"
cmake --build "$DIR/build"