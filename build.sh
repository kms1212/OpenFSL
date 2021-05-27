DIR="$( cd "$( dirname "$0" )" && pwd -P )"

mkdir "$DIR/build"
chown -R $USER:$USER "$DIR"

ls -l .

echo `cmake -B "$DIR/build" -S "$DIR"`
cmake -B "$DIR/build" -S "$DIR"
echo `cmake --build "$DIR/build"`
cmake --build "$DIR/build"