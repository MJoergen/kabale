#include <stack>
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>

#define NUM_QUADS 13

std::vector<int> cards;
std::vector<int> piles[4];

static int cards_hash()
{
   std::size_t seed = cards.size();
   for(auto& i : cards) {
      seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
   }
   return seed;
}


static void cards_print()
{
   for (int i=0; i<cards.size(); ++i)
      std::cout << cards[i] << " ";
   std::cout << std::endl;
}

static void cards_shuffle()
{
   // Initialize deck of cards
   for (int i=1; i<=NUM_QUADS; ++i) {
      for (int p=0; p<4; ++p) {
         cards.push_back(i);
      }
   }
   auto rng = std::default_random_engine {};
   std::shuffle(cards.begin(), cards.end(), rng);
}

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
}

static void piles_reorder()
{
   bool again = true;
   while (again) {
      again = false;
      for (int l=0; l<3; ++l) {
         int topl = piles[l].back();
         if (topl > 0) {
            for (int r=l+1; r<4; ++r) {
               int topr = piles[r].back();
               if (topl == topr) {
                  piles[r].push_back(topl);
                  piles[l].pop_back();
                  topl = piles[l].back();
                  again = true;
               }
            }
         }
      }
   } // while (again)
}

int main()
{
   cards_shuffle();

   while (cards.size() != 0) {
      cards_print();
      std::cout << cards_hash() << std::endl;

      // Reset piles.
      for (int p=0; p<4; ++p) {
         piles[p].clear();
         piles[p].push_back(-1);
      }

      // Deal deck ...
      for (auto it=cards.begin(); it!=cards.end();) {
         // Check if completed row
         if (it[0] == it[1] && it[1] == it[2] && it[2] == it[3]) {
            std::cout << "Finished card " << it[0] << std::endl;
            it += 4;
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

//      piles_print();
      // Recreate deck from piles
      cards.clear();
      for (int p=0; p<4; p++)
      {
         cards.insert(cards.end(), piles[p].begin()+1, piles[p].end());
      }
   } // while (true)
}

