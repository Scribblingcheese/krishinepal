import geocoder
from geopy.geocoders import Nominatim

def get_state():

    g = geocoder.ip('me')
    l = g.latlng

    coastal = ["srikakulam" , "vizianagaram" , "vishakhapatnam","east godavari" ,"west godavari" , "krishna" ,"guntur" ,"prakasam" ,"nellore"]
    rayalaseema = ["kurnool" ,"kadapa" ,"anantapur","chittor"]

    geolocator = Nominatim(user_agent="geoapiExercises")

    Latitude = str(l[0])
    Longitude = str(l[1])
 
    location = geolocator.reverse(Latitude+","+Longitude)
 
    address = location.raw['address']
 
    state = address.get('state', '')

    if state.upper() == "ANDHRA PRADESH":
        district = address.get('state_district')

        if district in coastal:
            state = "Coastal Andhra Pradesh"
        if district in rayalaseema:
            state = "RAYALSEEMA"


    return state