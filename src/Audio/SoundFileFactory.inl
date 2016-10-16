namespace Audio
{

	template <typename T> SoundFileReader* createReader() {return new T;}
// }

template <typename T>
void SoundFileFactory::registerReader(){
	// Make sure the same class won't be registered twice
	unregisterReader<T>();

	// Create a new factory with the functions provided by the class
	ReaderFactory factory;
	factory.check = &T::check;
	factory.create = &createReader<T>;

	// Add it
	s_readers.push_back(factory);
}

template <typename T>
void SoundFileFactory::unregisterReader(){
	// Remove the instance(s) of the reader from the array of factories
	for(ReaderFactoryArray::iterator it = s_readers.begin(); it != s_readers.end(); ){
		if(it->create == &createReader<T>)
			it = s_readers.erase(it);
		else
			++it;
	}
}

}
