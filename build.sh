DIR="$( cd "$( dirname "$0" )" && pwd -P )"

mkdir -p build
ls -l "$DIR"

cmake -B "$DIR/build" -S "$DIR"
cmake --build "$DIR/build"