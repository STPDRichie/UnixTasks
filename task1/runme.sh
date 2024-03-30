rm A 2>/dev/null
fallocate -l 4194305 A

gcc makeA.c -o makeA
./makeA

rm B 2>/dev/null

gcc sparse.c -o sparse

./sparse A B

gzip -k A
gzip -k B

gzip -cd B.gz | ./sparse C

./sparse A D 100

echo "Размер A: $(stat -c %s A) байт"
echo "Размер B: $(stat -c %s B) байт"
echo "Размер A.gz: $(stat -c %s A.gz) байт"
echo "Размер B.gz: $(stat -c %s B.gz) байт"
echo "Размер C: $(stat -c %s C) байт"
echo "Размер D: $(stat -c %s D) байт"
