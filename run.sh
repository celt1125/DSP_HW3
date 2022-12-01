
for i in $(seq 1 10)
do
	echo $i
	./mydisambig ./test_data/"$i"_sep.txt ZhuYin-Big5.map ngram_lm.txt ./result/"$i"_result.txt
done