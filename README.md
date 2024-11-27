# All words from all links found in a domain are saved into a file
Crowler in C, lcurl is needed, you have to install also lcurl.

Crowling domain:
  -cuts all style
  -cuts all script
  -cuts all HTML tags 
  -gets all words
  -gets all links 
  -formats all links
  -calls all links 
  -saves all words found in the html into a file

Compile in linux with:
gcc -o crowler crowler.c -lcurl
