#include <iostream>
#include <sdsl/suffix_arrays.hpp>
#include <fstream>
#include <streambuf>

using namespace std;
using namespace sdsl;

uint64_t count_runs(string &s){

	string bwt_s;

    cache_config cc;

    int_vector<8> text(s.size());
    assert(text.size()==s.size());

    for(uint64_t i=0;i<s.size();++i)
    	text[i] = (unsigned char)s[i];

    assert(text.size()==s.size());

    append_zero_symbol(text);

    store_to_cache(text, conf::KEY_TEXT, cc);

    construct_config::byte_algo_sa = LIBDIVSUFSORT;
    construct_sa<8>(cc);

    //now build BWT from SA
    int_vector_buffer<> sa(cache_file_name(conf::KEY_SA, cc));

    {

        for (uint64_t i=0; i<sa.size(); i++){
            auto x = sa[i];

            assert(x<=text.size());

            if ( x > 0 )
            	bwt_s.push_back((char)text[x-1]);
            else
            	bwt_s.push_back(0);

        }

    }



    sdsl::remove(cache_file_name(conf::KEY_TEXT, cc));
    sdsl::remove(cache_file_name(conf::KEY_SA, cc));

	uint64_t R = 1;

	char prev = bwt_s[0];

	for(uint64_t j=0;j<bwt_s.length();++j){

		char c = bwt_s[j];

		if(c != prev){

			++R;
			prev = c;

		}

	}

    return R;

}


/*
 * usage: count-bwt-runs input_file
 *
 */
int main(int argc, char** argv){

	if(argc!=2){

		cout << "Usage: count-bwt-runs input_file" << endl;

		exit(1);

	}


	string input_file = argv[1];

	ifstream t(input_file);
	string input((std::istreambuf_iterator<char>(t)),
	                 std::istreambuf_iterator<char>());

	uint64_t R = count_runs(input);

	cout << "Number of BWT runs: r = " << R << endl;

}
