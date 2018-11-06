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
    std::vector<size_t> x;
    size_t newLinks(0);
    
    for (size_t i=0; i < values.size(); ++i)
    {
        x.push_back(i);
    }
    
    for (size_t n(0); n < x.size(); ++n)
    {
        RNG.shuffle(x);
        unsigned int value = x[0];
        unsigned int nLinks(RNG.poisson(_a));
        unsigned int sum(0);
        unsigned int iteration(1);
        
        while (sum < nLinks and iteration < x.size()-1)
        {
            if (add_link(value,x[iteration])==true)
            {
                ++sum;
            }
            ++iteration;
        }
        newLinks += sum;
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
