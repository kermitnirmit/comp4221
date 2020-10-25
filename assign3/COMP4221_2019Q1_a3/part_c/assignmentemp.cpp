//
// Created by Dekai WU and YAN Yuchen on 20190417.
//

#include "assignment.hpp"
#include <shared_utils.hpp>

namespace part_c{
  const unsigned NUM_EPOCHS = 2;

  /**
   * create your custom classifier by combining transducers
   * \param training_set the training set that your classifier will train on
   * \param postag_vocab a list of all possible postags
   * \param iobes_syntactic_tag_vocab a list of all possible syntactic chunk tags (with IOBES prefix), like "I-NP" "O-NP" etc.
   * \param iobes_semantic_role_vocab a list of all possible semantic roles (with IOBES prefix), like "I-who" "O-who" etc.
   * \return a classifier object
   */
  transducer_t your_classifier(const vector <sentence_t> &training_set, const vector <symbol_t> &postag_vocab,
                               const vector <symbol_t> &iobes_syntactic_tag_vocab,
                               const vector <symbol_t> &iobes_semantic_role_vocab) {

    // in this starting code, we demonstrates how to construct a 2-layer feedforward neural network
    // that takes the target token, the predicate, the target token POS tag,
    // the target token IOBES shallow syntactic tag, and target-predicate distance as input

    // first you need to assemble the vocab you need
    // in this simple model, the vocab is the set of all tokens that appear in training set
    // we make use of std::unordered_set data structure to collect tokens, because it naturally removes duplicates
    std::unordered_set<symbol_t> token_set;
    for (const auto &sentence:training_set) {
      for (const auto &token:sentence) {
        token_set.insert(token);
      }
    }
    vector <symbol_t> vocab(token_set.begin(), token_set.end());

    // create an embedding lookup layer will convert token to tensor
    auto embedding_lookup = make_embedding_lookup(64, vocab);
    auto embedding_lookup2 = make_embedding_lookup(64, vocab);
    auto embedding_lookup3 = make_embedding_lookup(64, vocab);
    auto embedding_lookup4 = make_embedding_lookup(64, vocab);
    // create an 1-hot layer that is intended to handle the POS tag of current token as input
    // the size of postag vocabulary are small, a 1-hot layer will work just fine
    auto postag_onehot = make_onehot(postag_vocab);
    auto postag_onehot2 = make_onehot(postag_vocab);
    auto postag_onehot3 = make_onehot(postag_vocab);
    // create an 1-hot layer that is intended to handle the IOBES shallow syntactic tag of current token as input
    // the size of IOBES syntactic tag vocabulary are small, a 1-hot layer will work just fine
    auto iobes_syntactic_onehot = make_onehot(iobes_syntactic_tag_vocab);
    auto iobes_syntactic_onehot2 = make_onehot(iobes_syntactic_tag_vocab);
    auto iobes_syntactic_onehot3 = make_onehot(iobes_syntactic_tag_vocab);
    auto concatenate = make_concatenate(10);

    auto dense0 = make_dense_feedfwd(64, make_tanh());

    auto dense1 = make_dense_feedfwd(iobes_semantic_role_vocab.size(), make_softmax());

    auto onehot_inverse = make_onehot_inverse(iobes_semantic_role_vocab);
  //   return compose(
  //     group(
  //       embedding_lookup,
  //       embedding_lookup2,
  //       embedding_lookup3,
  //       embedding_lookup4), // handles the input - target token POS tag
  //     concatenate,
  //     dense0, dense1, onehot_inverse);
  // }
    return compose(
      group(
        embedding_lookup, // handles the input - target token
        embedding_lookup2,
        embedding_lookup3, // handles the input - predicate tokoen
        embedding_lookup4,
        postag_onehot,
        postag_onehot2,
        postag_onehot3,  // handles the input - target token POS tag
        iobes_syntactic_onehot,
        iobes_syntactic_onehot2,
        iobes_syntactic_onehot3, // handles the input - target token IOBES shallow syntactic tag
        make_identity()), // handles the input - predicate-target distance
      concatenate,
      dense0, dense1, onehot_inverse);
    }

  /**
   * TODO: fill in documentation for return
   * besides the target token to SRL label, your model may also need other "context" input
   * this function defines the inputs that your model expects
   * \param sentence the sentence where the token is in
   * \param postags the POS tags of the sentence (predicted by your model)
   * \param shallow_syntactic_tags the shallow syntactic tags of the sentence
   *        (predicted by your shallow syntactic parser, in IOBES format)
   * \param target_index the position of the target token to SRL label
   * \param predicate_index the position of the predicate
   * \return the list of features that your classifier expects
   */
  vector<feature_t> get_features(const vector<token_t> &sentence, const vector<symbol_t> &postags,
                                 const vector<symbol_t> &shallow_syntactic_tags, unsigned target_index,
                                 unsigned predicate_index) {

    // this starting code demonstrates how to define input as:
    // the target token, the predicate, the target token POS tag, the target token IOBES shallow syntactic tag, and target-predicate distance
    // return vector<feature_t>{sentence[target_index], sentence[predicate_index], postags[target_index], shallow_syntactic_tags[target_index], newdist};
    
    double dist = (double) target_index - predicate_index;
    double newdist = dist * dist;
    if (target_index > 0) {
      if (target_index < sentence.size() - 1) {
        return vector<feature_t>{sentence[target_index - 1], sentence[target_index], sentence[target_index + 1], sentence[predicate_index], postags[target_index - 1], postags[target_index], postags[target_index + 1], shallow_syntactic_tags[target_index - 1], shallow_syntactic_tags[target_index], shallow_syntactic_tags[target_index + 1], newdist};
        // return vector<feature_t>{sentence[target_index], sentence[predicate_index], postags[target_index - 1], postags[target_index], postags[target_index + 1], shallow_syntactic_tags[target_index], newdist};
      } else {
        // return vector<feature_t>{sentence[target_index - 1], sentence[target_index], "<S>", shallow_syntactic_tags[target_index]};
        // return vector<feature_t>{sentence[target_index], sentence[predicate_index], postags[target_index - 1], postags[target_index], "<S>", shallow_syntactic_tags[target_index], newdist};
        return vector<feature_t>{sentence[target_index - 1], sentence[target_index], "<S>", sentence[predicate_index], postags[target_index - 1], postags[target_index], "<S>", shallow_syntactic_tags[target_index - 1], shallow_syntactic_tags[target_index], "<S>", newdist};
      }
    } else {
      if (sentence.size() > 1) {
        // return vector<feature_t>{"<S>", sentence[target_index], sentence[target_index + 1], shallow_syntactic_tags[target_index]};
        // return vector<feature_t>{sentence[target_index], sentence[predicate_index], "<S>", postags[target_index], postags[target_index + 1], shallow_syntactic_tags[target_index], newdist};
        return vector<feature_t>{"<S>", sentence[target_index], sentence[target_index + 1], "<S>", sentence[predicate_index], postags[target_index], postags[target_index + 1], "<S>", shallow_syntactic_tags[target_index], shallow_syntactic_tags[target_index + 1], newdist};

      } else {
        // return vector<feature_t>{ "<S>", sentence[target_index], "<S>", shallow_syntactic_tags[target_index]};
        return vector<feature_t>{"<S>", sentence[target_index], "<S>", sentence[predicate_index], "<S>", postags[target_index], "<S>", "<S>", shallow_syntactic_tags[target_index], "<S>", newdist};
        // return vector<feature_t>{sentence[target_index], sentence[predicate_index], "<S>", postags[target_index], "<S>", shallow_syntactic_tags[target_index], newdist};
      }
    }
  }
}
