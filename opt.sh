logfile='opt.md'

echo "| option | time | wasm | js |" > $logfile
echo "| ------ | ---- | ---- | -- |" >> $logfile

options='0 1 2 3 s z'
# options='0'
for option in $options
do
    elapsed=$(/usr/bin/time -o /dev/stdout -f "%e" emcc elm_values.c -s WASM=1 -o elm_values.html -O$option )
    # time_js_expr=$(/usr/bin/time -o /dev/stdout -f "%S + %U" emcc elm_values.c -s WASM=1 -o elm_values.html -O$option )
    # cpu_time=$(node --print $time_js_expr) # not actually more stable, and less intuitive
    wasm_size=$(stat --printf="%s" elm_values.wasm)
    js_size=$(stat --printf="%s" elm_values.js)
    echo "| -O$option | $elapsed | $wasm_size | $js_size |" >> $logfile
done

cat $logfile
