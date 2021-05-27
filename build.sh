DIR="$( cd "$( dirname "$0" )" && pwd -P )"

mkdir "$DIR/build"
chown -R $USER:$USER "$DIR"

ls -l .

cmake -B "$DIR/build" -S "$DIR"
cmake --build "$DIR/build"