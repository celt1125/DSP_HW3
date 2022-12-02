
for i in $(seq 1 10)
do
	echo $i
	./mydisambig ./test_data/"$i"_sep.txt ZhuYin-Big5.map ngram_lm.txt ./result/"$i"_result.txt
	#disambig -text ./test_data/"$i"_sep.txt -map ZhuYin-Big5.map -lm ngram_lm.txt -order 2 > ./result/"$i"_ans.txt;
done

#echo diff result:
#for i in $(seq 1 10)
#do
#	echo $i
#	diff ./result/"$i"_result.txt ./result/"$i"_ans.txt
#done

