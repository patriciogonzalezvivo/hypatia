
# Load timezones.csv
import csv
import os
import sys

SECONDS_PER_MINUTE = 60
MINUTES_PER_HOUR = 60
HOURS_PER_DAY = 24
SECONDS_PER_DAY = SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY
MINUTES_PER_DAY = MINUTES_PER_HOUR * HOURS_PER_DAY
DAYS_PER_MINUTE = 1.0 / MINUTES_PER_DAY
DAYS_PER_SECOND = 1.0 / SECONDS_PER_DAY

continents = []
timezones = []
countries_name = []
countries_alpha2 = []
cities = []

def load_timezones():
    filename = os.path.join(os.path.dirname(__file__), 'timezones.csv');
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            # Skip header
            if row[0].startswith('timezone'):
                continue
            tz_name = row[0].strip()
            timezones.append(tz_name)

def load_countries():
    filename = os.path.join(os.path.dirname(__file__), 'countries.csv');
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            # Skip header
            if row[0].startswith('name'):
                continue
            
            country_name = row[0].strip()
            if country_name.startswith('"') and country_name.endswith('"'):
                print(f"{country_name} has quotes")
                country_name = country_name[1:-1]

            country_alpha2 = row[1].strip()
            country_continent = row[5].strip()
            if (country_continent not in continents):
                continents.append(country_continent)
            countries_name.append(country_name)
            countries_alpha2.append(country_alpha2)


def load_cities():
    filename = os.path.join(os.path.dirname(__file__), 'cities.csv');
    with open(filename
            , 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            # Skip header
            if row[0].startswith('country'):
                continue
            city_country = row[0].strip()
            city_state = row[1].strip()
            city_name = row[2].strip()

            # convert city_name characters into the ASCII range
            city_name = city_name
            city_name = city_name.encode('ascii', 'ignore').decode('ascii')
            
            city_lat = float(row[3].strip())
            city_lon = float(row[4].strip())
            city_tz = row[5].strip()
            city_alt_m = float(row[6].strip())

            if city_tz not in timezones:
                print(f"Timezone {city_tz} not found in timezones.csv")
                continue

            if city_country not in countries_alpha2:
                print(f"Country {city_country} not found in countries.csv")
                continue

            city_tz_index = timezones.index(city_tz)
            city_country_index = countries_alpha2.index(city_country)
            cities.append( [city_name, city_lat, city_lon, city_tz_index, city_country_index, city_alt_m] )

            # city[city_name] = [ city_lat, city_lon, city_tz_index, city_country, city_state ]
            # print(f"City {city_name} loaded with timezone {city_tz}")

load_timezones()
load_countries()
load_cities()

# open a file call countries.txt in write mode
with open('countries.h', 'w') as f:
    f.write("const size_t N_COUNTRIES = " + str(len(countries_name)) + ";\n")
    f.write("const char* countries[N_COUNTRIES] = {\n")
    for country in countries_name:
        f.write("(char*)\"" + country + "\", ")
    f.write("\n};\n")

    f.write("const char countries_alpha2[N_COUNTRIES][2] = {\n")
    for country in countries_alpha2:
        f.write("{\'" + country[0] + "\',\'" + country[1] + "\'},")
    f.write("\n};\n")

# open a file call cities.txt in write mode
with open('cities.h', 'w') as f:
    f.write("const size_t N_CITIES = " + str(len(cities)) + ";\n")
    f.write("const char* cities[N_CITIES] = {\n")
    for city in cities:
        f.write("(char*)\"" + city[0] + "\", ")
    f.write("\n};\n")

    f.write("const double cities_loc[N_CITIES][2] = {\n")
    for city in cities:
        f.write("{" + str(city[1]) + "," + str(city[2]) + "},")
    f.write("\n};\n")
    f.write("const size_t cities_tz[N_CITIES] = {\n")
    for city in cities:
        f.write(str(city[3]) + ",")
    f.write("\n};\n")
    f.write("const size_t cities_country[N_CITIES] = {\n")
    for city in cities:
        f.write(str(city[4]) + ",")
    f.write("\n};\n")

    f.write("const double cities_alt[N_CITIES] = {\n")
    for city in cities:
        f.write(str(int(city[5])) + ",")
    f.write("\n};\n")