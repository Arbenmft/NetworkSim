#include "network.h"
#include "random.h"

void Network::resize(const size_t& size)
{
    values.clear();
    for (size_t i(0); i < size; ++i)
    {
        values.push_back(RNG.normal(0,1));
    }
}

bool Network::add_link(const size_t& _a , const size_t& _b)
 {
     
    if (_a >= values.size() or _b >= values.size())
    {
        return false;
    }
    
    if (_a == _b)
    {
        return false;
    }
    
    auto result = links.equal_range(_a);
    for (auto it = result.first; it != result.second; it++)
    {
        if (it->second == _b)
        {
            return false;
        }
     }
     
     result = links.equal_range(_b);
     for (auto it = result.first; it != result.second; it++)
     {
         if (it->second == _a)
         {
             return false;
         }
     }
     
    links.insert(std::pair<size_t, size_t>(_a, _b));
    links.insert(std::pair<size_t,size_t>(_b,_a));
    
    return true;
     
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
    
    
    for (size_t t=0; t < x.size(); ++t)
    {
        size_t firstValue = x[t];
        size_t degree(RNG.poisson(_a));
        
        for (size_t n=0; n < degree; ++n)
        {
           if(add_link(firstValue,x[n])==true)
           {
               ++newLinks;
               RNG.shuffle(x);
           }
        }
    }
    return newLinks;
}

size_t Network::set_values(const std::vector<double> &_vect)
{
	size_t total(0);
    values.clear();
    std::vector<double> x = values;
    
    for (size_t i=0;i<_vect.size(); ++i)
    {
        values.push_back(_vect[i]);
        ++ total;
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
	std::sort(x.rbegin(), x.rend());
	
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
