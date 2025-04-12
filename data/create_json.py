
# make a python script that creates a json from the cities.csv, which contains the name of the city, the state, and the country and their lat and long to then be use by the JS frontend in a fuzzy search

import csv
import json

# Create a list to hold the city data
cities = []

FILE = 'cities.csv'
# FILE = 'cities_with_state.csv'

# Open the CSV file and read the data
with open(FILE, 'r') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        # Create a dictionary for each city and append it to the list

        # sanizite the string for unicode
        row['name'] = row['name'].encode('utf-8').decode('utf-8')
        # row['state'] = row['state'].encode('utf-8').decode('utf-8')
        row['country'] = row['country'].encode('utf-8').decode('utf-8')

        # lat and lng need to be save as double 
        row['lat'] = float(row['lat'])
        row['lng'] = float(row['lng'])
        city = {
            'name': row['name'],
            # 'state': row['state'],
            'country': row['country'],
            'lat': row['lat'],
            'long': row['lng']
        }
        cities.append(city)

# Write the list of cities to a JSON file
with open('cities.json', 'w') as jsonfile:
    json.dump(cities, jsonfile, indent=4)

print("JSON file created successfully.")


