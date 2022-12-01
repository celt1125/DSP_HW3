# separate files
for i in $(seq 1 10)
do
	echo $i
	perl ./separator_big5.pl ./test_data/"$i".txt > ./test_data/"$i"_sep.txt
done

# setup language model
perl ./separator_big5.pl ./corpus.txt > ./corpus_sep.txt
ngram-count ‒text ./corpus_sep.txt ‒write ./ngram_cnt.txt ‒order 2
ngram-count ‒read ./ngram_cnt.txt ‒lm ./ngram_lm.txt ‒order 2 -unk