# Motion

Goal of the project is to check if a string is "meaningful" and "translatable" and output the translated message if both satisfied.

Meaningful (return 1 if not meaningful):
'/': empty beat
'a''s''w''d': four directions, must followed by a slash '/'
0-9: digits, 0~99 followed by a direction; >=100 not allowed (acceptable: 1 digit followed by a direction, 2 consecutive digits followed by a direction; Not acceptable: 3 or more consecutive digits )

Translatable:
return 2: #slashes != digits  eg. 4a must be followed by >=4 '/': good: 4a//// bad: 4a//s/w//
return 3: above problem occurs at the end of the string: eg. 3s///4a/
return 4: the digit <=1: eg. 1s/, 01/s, 0s/
return 0: translatable

eg. 
1> check meaningful (not meaningful, return 1): 
es/    → input not included in the list.
a/sw/  → a direction not followed by a slash.
8/a/   → a digits followed by neither a direction nor a slash.
8aw    → a digit followed by a direction but not followed by a slash.
123a/  → two digits not followed by a direction.
12aw   → two digits followed by a direction but not followed by a slash.

2> meaningful, not translatable:
Return 4 (length < 1): 3w////1d/, 2a///01w//3s//, 3a////0d/, 00s/ 
Return 3 (end prematurely): 3w//, 3w///4s//, 02w/, 10s///, 2w//04s//, 2w//12w///, 23s//, 3w///23s//
Return 2 (#slash doesn’t match the digits): 04s//2a///, 2w///12a///2s//, 3s//2w///, 04a/////23w//3s///, 23w//3s//
Return 0 (translatable): ////, //a/W//, 2w///3s///02a///, zero input, 12w/////////////////
