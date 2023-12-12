name="$1"
if [ "$#" -ne 1 ]; then
	name="httpc"
	echo "Output file is $name"
fi

echo "Compiling (static) binary using $CC to $name..."
$CC -o $name src/*.c -static
echo "Done. running 'strip -s' on $name..."
strip -s infs
echo "Done!\nrun ./$name -h"
