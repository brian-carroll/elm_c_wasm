while true
do
  make -q dist/bin/test || make check-bin $@ || true
  sleep 1
done
