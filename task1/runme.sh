make clean

fallocate -l 4194305 A

gcc makeA.c -o makeA
./makeA

gcc sparse.c -o sparse

./sparse A B

gzip -k A
gzip -k B

gzip -cd B.gz | ./sparse C

./sparse A D 100

echo "Размер A: $(stat -c %s A) байт" >> ./result.txt
echo "Размер B: $(stat -c %s B) байт" >> ./result.txt
echo "Размер A.gz: $(stat -c %s A.gz) байт" >> ./result.txt
echo "Размер B.gz: $(stat -c %s B.gz) байт" >> ./result.txt
echo "Размер C: $(stat -c %s C) байт" >> ./result.txt
echo "Размер D: $(stat -c %s D) байт" >> ./result.txt
