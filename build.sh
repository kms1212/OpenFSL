DIR="$( cd "$( dirname "$0" )" && pwd -P )"

cmake -B "$DIR/build" -S "$DIR"
cmake --build "$DIR/build"