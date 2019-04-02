import urllib.request
from bs4 import BeautifulSoup

# Request and parse the web
with urllib.request.urlopen("http://www.shopping.com/camera/products?CLT=SAS&KW=camera") as u:
    soup = BeautifulSoup(u.read(), 'html.parser')

# Find all images
for i in soup.find_all("div", {"class": "gridItemTop"}):
    test = i.find("span", {"class": "placeholderImg"})
    if test:
        url = test.text
    else:
        url = i.find("img")["src"]
    print(url.split("?")[0])
