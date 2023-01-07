# All words from all links found in a domain are saved into a file
Using lcurl, you need to install first lcurl

Crowling domain: www.domain.com
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
