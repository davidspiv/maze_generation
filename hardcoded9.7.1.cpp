#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main()
{
   string input_file_name;
   cin >> input_file_name;

   ofstream out;
   out.open("sentences.txt");
   if (input_file_name == "in1.txt")
   {
      out << R"("I am afraid, Watson, that I shall have to go," said Holmes, as we sat down together to our breakfast one morning.
"Go!
Where to?"
"To Dartmoor; to King's Pyland."
I was not surprised.
Indeed, my only wonder was that he had not already been mixed up in this extraordinary case, which was the one topic of conversation through the length and breadth of England.
For a whole day my companion had rambled about the room with his chin upon his chest and his brows knitted, charging and recharging his pipe with the strongest black tobacco, and absolutely deaf to any of my questions or remarks.
Fresh editions of every paper had been sent up by our news agent, only to be glanced over and tossed down into a corner.
Yet, silent as he was, I knew perfectly well what it was over which he was brooding.
There was but one problem before the public which could challenge his powers of analysis, and that was the singular disappearance of the favorite for the Wessex Cup, and the tragic murder of its trainer.
When, therefore, he suddenly announced his intention of setting out for the scene of the drama it was only what I had both expected and hoped for.
"I should be most happy to go down with you if I should not be in the way," said I.
"My dear Watson, you would confer a great favor upon me by coming.
And I think that your time will not be misspent, for there are points about the case which promise to make it an absolutely unique one.
We have, I think, just time to catch our train at Paddington, and I will go further into the matter upon our journey.
You would oblige me by bringing with you your very excellent field-glass."
And so it happened that an hour or so later I found myself in the corner of a first-class carriage flying along en route for Exeter, while Sherlock Holmes, with his sharp, eager face framed in his ear-flapped travelling-cap, dipped rapidly into the bundle of fresh papers which he had procured at Paddington.
We had left Reading far behind us before he thrust the last one of them under the seat, and offered me his cigar-case.
)";
   }
   else if (input_file_name == "in2.txt")
   {
      out << R"(Happy families are all alike; every unhappy family is unhappy in its own way.
Everything was in confusion in the Oblonskys’ house.
The wife had discovered that the husband was carrying on an intrigue with a French girl, who had been a governess in their family, and she had announced to her husband that she could not go on living in the same house with him.
This position of affairs had now lasted three days, and not only the husband and wife themselves, but all the members of their family and household, were painfully conscious of it.
Every person in the house felt that there was no sense in their living together, and that the stray people brought together by chance in any inn had more in common with one another than they, the members of the family and household of the Oblonskys.
The wife did not leave her own room, the husband had not been at home for three days.
The children ran wild all over the house; the English governess quarreled with the housekeeper, and wrote to a friend asking her to look out for a new situation for her; the man-cook had walked off the day before just at dinner time; the kitchen-maid, and the coachman had given warning.
)";
   }
   out.close();

   return 0;
}
