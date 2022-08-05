#name: Dennis Preston Beaty
#netid: dbeaty

import re
import csv
import requests
from bs4 import BeautifulSoup
import numpy as np 
import matplotlib.pyplot as plt

#First column holds url Second Holds Name of Book
csv_data = list(csv.reader(open('sitesToScrap.csv')))

#word_dict is the master dictionary that contains all unique words on a book to book basis
word_dict = {}
#book_word_dict contains the words from each book and how often they occur
book_word_dict = []
#uniq_book_word_dict contains the words unique to each book and how often they occur
uniq_book_word_dict = []

#Loop through the csv_data to obtain the book
for i in range(len(csv_data)):
    #obtain the book
    csv_data[i][0] = BeautifulSoup(requests.get(csv_data[i][0]).content, 'html.parser').get_text()

    #remove non-bookended words
    left_strip = csv_data[i][0].index("*** START OF THE PROJECT GUTENBERG EBOOK ") + len("*** START OF THE PROJECT GUTENBERG EBOOK " + csv_data[i][1].upper() + " ***") + 1
    right_strip = csv_data[i][0].index("*** END OF THE PROJECT GUTENBERG EBOOK ")
    #remove book heading and endings
    csv_data[i][0] = csv_data[i][0][left_strip:right_strip]
    #make text lowercase
    csv_data[i][0] = csv_data[i][0].lower()
    #remove specific characters
    remove_characters = [',','"','.','&','|',':','@',',','<','>','(',')','*','$','?','!','/',';','=','”','“','‘','[',']','"\\"']
    stopwords = ['ut', '\'re','.', ',', '--', '\'s','cf', '?', ')', '(', ':','\'','\"', '-', '}','â','£', '{', '&', '|', u'\u2014', '', ']' ]
    stopnums = ['0','1','2','3','4','5','6','7','8','9']

    csv_data[i][0] = csv_data[i][0].replace("—", ' ')
    for x in range(len(remove_characters)):
        csv_data[i][0] = csv_data[i][0].replace(remove_characters[x], '')
    csv_data[i][0] = csv_data[i][0].replace("\r\n", ' ')
    #partion csv_data[i][0] into list of strings
    csv_data[i][0] = csv_data[i][0].split()
    csv_data[i][0] = [z for z in csv_data[i][0] if z not in stopwords]
    csv_data[i][0] = [z for z in csv_data[i][0] if z not in stopnums]
    csv_data[i][0] = [z for z in csv_data[i][0] if not z.isdigit()]
    #create an array of unique words per book
    unique_word = []
    [unique_word.append(x) for x in csv_data[i][0] if x not in unique_word]

    book_diction = {}

    #make dictionary per book
    for k in csv_data[i][0]:
        if (k) in book_diction.keys():
            book_diction[k] += 1
        else:
            book_diction[k] = 1

    book_word_dict.append(book_diction)
    #count how many times a word is used on a book to book basis within dictionary
    for k in unique_word:
        if k in word_dict.keys():
            word_dict[k] += 1
        else:
            word_dict[k] = 1

#remove dups from word dictionary
temp_dict = {}
for key in word_dict:
    if (word_dict[key] == 1):
        temp_dict[key] = word_dict[key]
word_dict = temp_dict

#create unique value uniquedictionaries
for i in book_word_dict:
    temp_dict = {}
    for j in word_dict.keys():
        if j not in i.keys():
            continue
        temp_dict[j] = i[j]
    uniq_book_word_dict.append(temp_dict)
#sort by most used
for i in range(len(book_word_dict)):
    book_word_dict[i] = sorted(book_word_dict[i].items(), key=lambda x: x[1], reverse = True)

for i in range(len(uniq_book_word_dict)):
    uniq_book_word_dict[i] = sorted(uniq_book_word_dict[i].items(), key=lambda x: x[1], reverse = True)
#create text files for unique words in each book
counter = 0
select_print = "'"
for diction in uniq_book_word_dict:
    book_name= csv_data[counter][1]
    fn = book_name + ".txt"
    file = open(fn, mode="w+")
    for i in range(25):
        #select way to write to file depending on whether or not there is an apostrophe
        if select_print in diction[i][0]:
            file.write('("' + diction[i][0] + '", ' + str(diction[i][1]) + ")\n")

        else:
            file.write("('" + diction[i][0] + "', " + str(diction[i][1]) + ")\n")
    file.close()
    counter += 1
    
#create the plots for word frequency
uniq_graph_x_val = []
uniq_graph_y_val = []

graph_x_val = []
graph_y_val = []
#generate values to be graphed
for i in range(25):
    uniq_graph_x_val.append(uniq_book_word_dict[0][i][0])
    uniq_graph_y_val.append(uniq_book_word_dict[0][i][1])

    graph_x_val.append(book_word_dict[0][i][0])
    graph_y_val.append(book_word_dict[0][i][1])
#create graph for unique values
fig1, (axis1,axis2) = plt.subplots(1,2)

axis2.barh(uniq_graph_x_val, uniq_graph_y_val, color = 'red')
axis2.grid(b = True, color ='grey',linestyle ='-.', linewidth = 0.4)
axis2.invert_yaxis()
#change axis titles and labels 
axis2.set_xlabel("Words Used")
axis2.set_ylabel("# of Times Used")
axis2.title.set_text("Unique Word Usage in \"Around the World in Eighty Days\"")

#create graph for all values

axis1.barh(graph_x_val, graph_y_val, color = 'blue')
axis1.grid(b = True, color ='grey',linestyle ='-.', linewidth = 0.4)
axis1.invert_yaxis()
#change axis titles and labels 
axis1.set_xlabel("Words Used")
axis1.set_ylabel("# of Times Used")
axis1.title.set_text("Word Usage in \"Around the World in Eighty Days\"")
plt.tight_layout()


plt.show()
#create graph for all values