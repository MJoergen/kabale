#include <set>
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>
#include <assert.h>

#define NUM_QUADS 13

std::vector<int> cards;
std::vector<int> piles[4];
auto rng = std::default_random_engine {};


static int cards_hash()
{
   int seed = cards.size();
   for(auto i : cards) {
      seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
   }
   return seed;
} // cards_hash


static void cards_print()
{
   for (int i=0; i<cards.size(); ++i)
   {
      assert(cards[i] > 0);
      std::cout << cards[i] << " ";
   }
   std::cout << std::endl;
} // cards_print


static void cards_shuffle()
{
   // Initialize deck of cards
   cards.clear();
   for (int i=1; i<=NUM_QUADS; ++i) {
      for (int p=0; p<4; ++p) {
         cards.push_back(i);
      }
   }
   std::shuffle(cards.begin(), cards.end(), rng);
} // cards_shuffle


static void piles_print()
{
   bool more = true;
   for (int r=0; more; ++r)
   {
      more = false;
      for (int j=0; j<4; ++j)
      {
         if (piles[j].size() > r)
         {
            std::cout << std::setw(3) << piles[j][r];
            more = true;
         } else {
            std::cout << "  .";
         }
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
} // piles_print


static void piles_reorder()
{
   bool again = true;
   while (again) {
      again = false;
      for (int l=0; l<3; ++l) {
         int topl = piles[l].back();
         for (int r=l+1; r<4; ++r) {
            int topr = piles[r].back();
            if (topl > 0 && topl == topr) {
               piles[r].push_back(topl);
               piles[l].pop_back();
               topl = piles[l].back();
               again = true;
            }
         }
      }
   }
} // piles_reorder


// Returns true if game was solved.
// Returns false if infinite loop was detected.
static bool game_play()
{
   cards_shuffle();

   std::set<int> hashes;

   while (cards.size() != 0) {
      int hash = cards_hash();
      if (hashes.find(hash) != hashes.end()) {
         return false;
      }
      hashes.insert(hash);

      // Reset piles.
      for (int p=0; p<4; ++p) {
         piles[p].clear();
         piles[p].push_back(-1);
      }

      // Deal deck ...
      for (auto it=cards.begin(); it!=cards.end();) {
         // Check if completed row
         if (it[0] == it[1] && it[1] == it[2] && it[2] == it[3]) {
            it += 4;
            hashes.clear();
            // Don't push cards to piles
            continue;
         }
         // ... one row at a time.
         for (int p=0; p<4; ++p) {
            piles[p].push_back(*it++);
         }

         // Reorder piles after each row.
         piles_reorder();
      } // for

      // Recreate deck from piles
      cards.clear();
      for (int p=0; p<4; p++)
      {
         cards.insert(cards.end(), piles[p].begin()+1, piles[p].end());
      }
   } // while (true)
   return true;
} // game_play



int main()
{
   int solved = 0;
   for (int i=0; i<100000; ++i)
   {
      bool res = game_play();
      solved += res;
   }

   std::cout << solved << std::endl;
} // main

