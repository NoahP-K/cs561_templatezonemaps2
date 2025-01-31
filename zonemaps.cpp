#include "zonemaps.h"

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint _num_elements_per_zone)
{
    // constructor 
    // Your code starts here ...

    //Initialize instance vars
    elements = _elements;
    zones = {};
    num_zones =  0;
    num_elements_per_zone = _num_elements_per_zone;

    //Iterate over all elements, adding to a zone until it is full.
    uint esize = elements.size();\

    zone<T> newZone;
    newZone.size = 0;
    newZone.elements = {};
    for(int i = 0; i < esize; i++) {
        uint e = elements[i];
        //compare to the max, reassign if needed
        if(newZone.size == 0 || e > newZone.max) {
            newZone.max = e;
        }
        //compare to the min, reassign if needed
        if(newZone.size == 0 || e < newZone.min) {
            newZone.min = e;
        }
        newZone.elements.push_back(e);
        newZone.size++;
        //add the new zone to the vector early if there are no more elements to add. 
        if (newZone.size == num_elements_per_zone || i == esize - 1) {
            zones.push_back(newZone);
            //reset newZone to begin making a new zone
            newZone = zone<T>();
            //std::cout << "zone made" << std::endl;
        }
    }
}

template<typename T>
size_t zonemap<T>::query(T key)
{
    // Your code starts here ...
    //Loop through the zones until finding one with the right range, then loop through its elements.
    //Every time an element matching the key is found, add to the count.
    size_t count = 0;
    for(int i = 0; i < zones.size(); i++) {
        zone<T> z = zones[i];
        if(z.min <= key && z.max >= key) {
            //It is necessary to not stop searching zones after finding one that fits the key, even with sorted data.
            //If, for example, the max of one zone equalled the min of the next zone and the key was that value
            //  then two zones would need to be scanned. 
            for(int j = 0; j < z.elements.size(); j++) {
                T e = z.elements[j];
                count += (e == key);
            }
        }
    }
    return count;
}

template<typename T>
std::vector<T> zonemap<T>::query(T low, T high) {
    //std::cout << "Range query range: " << low << ", " << high << std::endl;
    //functions very similarly to the other query method but with more complex checks for key matching and valid
    //  element checking.
    std::vector<T> results = {};
    int zoneMatches = 0;
    for(int i = 0; i < zones.size(); i++) {
        zone<T> z = zones[i];
        //std::cout << "Zone: " << z.min << ", " << z.max << std::endl;
        if(low <= z.max && z.min <= high) {
            zoneMatches++;
            //It is necessary to not stop searching zones after finding one that fits the keys, even with sorted data.
            //A single range of values could intersect multiple zones. 
            for(int j = 0; j < z.elements.size(); j++) {
                T e = z.elements[j];
                if(e <= high && e >= low) {
                    results.push_back(e);
                }
            }
        }
    }
    //std::cout << "Range query zone matches: " << zoneMatches << std::endl;
    return results;
}

template<typename T>
void zonemap<T>::build()
{
    //As the zonemap has no methods to incrementally add/remove elements, having a specified build method seemed 
    //  superfluous. I placed this functionality directly in the constructor instead. 
}

template<typename T>
void zonemap<T>::sort_elements()
{
    //According to a response on piazza, this did not need to be implemented. 
}