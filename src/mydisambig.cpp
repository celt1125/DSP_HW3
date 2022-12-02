#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <float.h>
#include "Ngram.h"

using namespace std;

// Global variables
unordered_map<string, vector<string>> zhuyin_map;

// Functions
string Viterbi(string);
void readMapFile(const char*);
vector<string> separateString(string);

int main(int argc, char **argv){
	const char* text_file = argv[1];
	const char* map_file = argv[2];
	const char* lm_file = argv[3];
	const char* out_file = argv[4];
	
	/* Read all files */
	readMapFile(map_file);
	ofstream out;
	out.open(out_file);
	ifstream in;
	in.open(text_file);

	/* Read language model*/
	Vocab voc;
	Ngram lm(voc, 2);
	File lmFile(lm_file, "r");
	lm.read(lmFile);
	lmFile.close();

	/* Start query */
	string buffer;
	while (getline(in, buffer, '\n')) {
		vector<string> query = separateString(buffer);
		int len = query.size() + 2;
		
		vector<vector<string>> candidates;
		vector<vector<double>> dp(len);
		vector<vector<int>> back_track(len);

		/* Initialize arrays */
		candidates.push_back(vector<string>{"<s>"});
		for (int i=0; i<query.size(); i++){
			candidates.push_back(zhuyin_map[query[i]]);
		}
		candidates.push_back(vector<string>{"</s>"});
		
		for (int i=0; i<len; i++) {
			dp[i].resize(candidates[i].size());
			back_track[i].resize(candidates[i].size());
		}

		/* DP */
		for (int i=1; i<len; i++) { // word position
			for (int j=0; j<candidates[i].size(); j++) { // candidates
				VocabIndex cand_curr = voc.getIndex(candidates[i][j].c_str());
				if (cand_curr == Vocab_None) {
					cand_curr = voc.getIndex(Vocab_Unknown);
				}
				double best_prob = -DBL_MAX;
				int best_back_idx = 0;
				for (int k=0; k<candidates[i-1].size(); k++) { // from last word position
					VocabIndex cand_last = voc.getIndex(candidates[i-1][k].c_str());
					if (cand_last == Vocab_None) {
						cand_last = voc.getIndex(Vocab_Unknown);
					}
					VocabIndex context[] = {cand_last, Vocab_None};
					// cout << "check: " << i-1 << ':' << dp.size() << ", " << j << ',' << k << ':' << dp[i-1].size() << '\n';
					double combine_prob = dp[i-1][k] + lm.wordProb(cand_curr, context);
					if (combine_prob > best_prob) {
						best_back_idx = k;
						best_prob = combine_prob;
					}
				}
				dp[i][j] = best_prob;
				back_track[i][j] = best_back_idx;
			}
		}

		/* Back tracking */
		// cout << "back tracking: " << back_track[len - 1].size() << '\n';
		vector<string> answer(len);
		int path_idx = 0;
		for (int i=len-1; i>=0; i--) {
			answer[i] = candidates[i][path_idx];
			path_idx = back_track[i][path_idx];
		}
		
		for (int i=0; i<len; i++) {
			out << answer[i] << " \n"[i==len-1];
		}
	}
	in.close();
	out.close();

    return 0;
}

string Viterbi(string line) {
	return " ";
}

void readMapFile(const char *map_file) {
	ifstream in;
	in.open(map_file);
	
	string buffer;
	while (getline(in, buffer, '\n')) {
		int pos = buffer.find_first_of("\t");

		vector<string> value = separateString(buffer.substr(pos+1));

		zhuyin_map[buffer.substr(0,pos)] = value;

		/* Check */
		// cout << buffer.substr(0,pos) << '\n';
		// for (int i=0; i<zhuyin_map[buffer.substr(0,pos)].size(); i++) {
		// 	cout << zhuyin_map[buffer.substr(0,pos)][i] << '\n';
		// }
	}
	in.close();
}

vector<string> separateString(string buffer) {
	int len = buffer.size();
	
	vector<string> value;
	for (int i=0; i<len; i++) {
		if (buffer[i] != ' '){
			value.push_back(buffer.substr(i, 2));
			i += 2;
		}
	}
	return value;
}
