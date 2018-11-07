#include <iostream>
#include "network.h"
#include "random.h"
#include <string>

void Network::resize(const size_t &size)
{
    values.clear();
    for (size_t i(0); i < size; ++i)
    {
        values.push_back(RNG.normal(0,1));
    }
}

bool Network::add_link(const size_t& _a , const size_t& _b)
{
    try
    {
        if (_a > values.size()-1 or _b > values.size()-1)
        {
            throw std::string("Value of the first parameter or second one is bigger than the size of the vector");
        }
        if (_a == _b)
        {
            throw std::string("The first paramater is equal to the second one");
        }
        auto result = links.equal_range(_a);
        for (auto it = result.first; it != result.second; it++)
        {
            if (it->second == _b)
            {
                throw std::string("The first parameter is already linked to the second one");
            }
        }
        result = links.equal_range(_b);
        for (auto it = result.first; it != result.second; it++)
        {
            if (it->second == _a)
            {
                throw std::string("The second paramter is already linked to the first one");
            }
        }
        links.insert(std::pair<size_t, size_t>(_a, _b));
        links.insert(std::pair<size_t,size_t>(_b,_a));
        return true;
    }
    
    catch(const std::string &e)
    {
        //std::cerr<< e << std::endl; //Affichage des messages d'erreur
        return false;
    }
 }
 
size_t Network::random_connect(const double& _a)
{
    links.clear();
    unsigned int newLinks(0);
    std::vector<size_t> index;
    
    for (size_t i(0); i < this->size(); ++i)
    {
        index.push_back(i);
    }
    
    try {
        for (size_t i(0); i< index.size(); ++i)
        {
            size_t degree(RNG.poisson(_a));
            if(degree > index.size())
            {
                throw std::string("Degree is bigger than the size of the vector");
            }
            
            RNG.shuffle(index);
            size_t iterator(0);
            
            for(int n(0); n < degree; ++n)
            {
                while(not add_link(i,index[iterator]))
                {
                    ++iterator;
                    continue;
                }
                newLinks++;
            }
        }
    }
    catch(std::string const& e)
    {
        std::cerr << e << std::endl;
        return 0;
    }
    return newLinks;
}

size_t Network::set_values(const std::vector<double> &_vect)
{
    unsigned int total(0);
    unsigned int size (std::min(values.size(),_vect.size()));
    for (size_t i(0); i < size; ++i)
    {
        values[i]=_vect[i];
        ++total;
    }
    return total;
}

size_t Network::size() const
{
	return values.size();
}

size_t Network::degree(const size_t& _n) const
{
	return links.count(_n);
}

double Network::value(const size_t& _n) const
{
	return values[_n];
}

std::vector<double> Network::sorted_values() const
{
	std::vector<double> x = values;
	std::sort(x.begin(), x.end(),std::greater<double>());
	return x;
}

std::vector<size_t> Network::neighbors(const size_t& _n) const
{
	std::vector<size_t> totalNeighbors;
	auto result = links.equal_range(_n);
	
    for (auto it = result.first; it != result.second; it++)
	{
		totalNeighbors.push_back(it->second);
	}
	return totalNeighbors;
}
