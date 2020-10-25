#include "assignment.hpp"

using namespace tg;

namespace part_review {
  const unsigned NUM_EPOCHS = 5;

vector<string> capital = {"true", "false"};
vector<token_t> token_vocab;
vector<token_t> suffixes;
/**
 * create your custom classifier by combining transducers
 * the input to your classifier will a list of tokens
 * \param vocab a list of all possible words
 * \param postags a list of all possible POS tags
 * \return
 */
void remove_duplicate(std::vector<token_t> vec) {
  std::sort(vec.begin(), vec.end());
  vec.erase(unique(vec.begin(), vec.end()), vec.end());
}
transducer_t your_classifier(const vector<token_t> &vocab, const vector<postag_t> &postags) {
   
  remove_duplicate(token_vocab);
    //remove_duplicate(suffixes);
  // in this starting code, we demonstrates how to construct a 2-layer feedforward neural network
  // that takes 2 tokens as features
   
  // embedding lookup layer is mathematically equivalent to
  // a 1-hot layer followed by a dense layer with identity activation
  // but trains faster then composing those two separately
  auto embedding_lookup = make_embedding_lookup(1, capital);
  auto embedding_lookup2 = make_embedding_lookup(64, suffixes);
  auto embedding_lookup3 = make_embedding_lookup(64,token_vocab);
  auto embedding_lookup4 = make_embedding_lookup(64,token_vocab);
  auto embedding_lookup5 = make_embedding_lookup(64,token_vocab);
   
  // create a concatenation operation
  // this operation can concatenate the 2 tokens (in 1-hot representation) into a big vector feature
   auto concatenate = make_concatenate(5);
   
  // create the first feedforward layer,
  // with 64 output units and tanh as activation function
  auto dense0 = make_dense_feedfwd(64, make_tanh());
   
  // create another feedforward layer
  // this is the final layer. so this layer should return the predicted POS tag (in 1-hot representation)
  // the output dimension of your final layer should be the size of all POS tags
  // because each dimension corresponds to a particular choice
  auto dense1 = make_dense_feedfwd((unsigned) postags.size(), make_softmax());
   
  // this is the inverse 1-hot operation
  // it takes a 1-hot vector feature, and returns a token from a pre-defined vocabulary
  // the 1-hot vector feature doesn't have to be perfect 0 and 1 values.
  // but they have to sum up to 1 (just like probability distribution)
  // usually the this (approximated) 1-hot input comes from a softmax operation
  auto onehot_inverse = make_onehot_inverse(postags);
   
  // connect these layers together
  // composing A and B means first apply A, and then take the output of A and feed into B
  return compose(group(embedding_lookup, embedding_lookup2, embedding_lookup3, embedding_lookup4, embedding_lookup5), concatenate, dense1, onehot_inverse);
}

// rename me into your_classifier if you want to use this classifier
  transducer_t your_classifier_knn(const vector<token_t> &vocab, const vector<postag_t> &postags) {

    // in this starting code, we demonstrates how to construct a KNN that takes two tokens as features

    // a KNN classifier takes a real-valued vector feature and directly returns the predicted class
    auto knn = make_symbolic_k_nearest_neighbors_classifier(5, 2, postags);

    return knn;
  }


std::string getsuffix(string s) {
  if (s.size() >= 3) {
    return s.substr(s.size() - 3);
  } else
  {
    return s;
  }
}
 
 
vector<token_t> get_features(const vector<token_t> &sentence, unsigned token_index) {
  string temp = sentence[token_index];
  string suffix = getsuffix(temp);
  suffixes.push_back(suffix);
  token_vocab.push_back(temp);
  string prevword;
  string nextword;
  if (token_index > 0) {
    if(token_index < sentence.size()-1) {
      prevword = sentence[token_index - 1];
      nextword = sentence[token_index + 1]; 
    }
    else {
      prevword = sentence[token_index-1];
      nextword = "<s>";
    }
  } else {
    if(sentence.size() > 1) {
      prevword = "<s>";
      nextword = sentence[token_index+1];
    }	
    else {
      prevword = "<s>";
      nextword = "<s>";
    }
  }
 
  if (isupper(sentence[token_index][0]) == 256) {
    return vector<token_t>{"true", suffix, prevword, temp, nextword};
  } else {
    return vector<token_t>{"false", suffix, prevword, temp, nextword};
  }
}

}
