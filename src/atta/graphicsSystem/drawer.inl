//--------------------------------------------------
// Atta Graphics System
// drawer.inl
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
	// Draw 3d objects
	template <typename T>
	void Drawer::add(T obj, StringId group) { getInstance().addImpl<T>(obj, group); }
	template <typename T>
	void Drawer::clear(StringId group) { getInstance().clearImpl<T>(group); }

	// Get data
	template <typename T>
	std::vector<T>& Drawer::get() { return getInstance().getImpl<T>(); }
	template <typename T>
	std::map<StringHash, std::vector<T>>& Drawer::getGroups() { return getInstance().getGroups<T>(); }
	template <typename T>
	unsigned Drawer::getMaxNumber() { return getInstance().getMaxNumberImpl<T>(); }
	template <typename T>
	unsigned Drawer::getCurrNumber() { return getInstance().getCurrNumberImpl<T>(); }

	// Draw
	template <typename T>
	void Drawer::draw() { getInstance().drawImpl<T>(); }

	// Draw 3d objects implementation
	template <typename T>
	void Drawer::addImpl(T obj, StringId group)
	{
		if constexpr(std::is_same<T, Drawer::Line>::value || std::is_same<T, Drawer::Point>::value)
		{
			getGroupsImpl<T>()[group.getId()].push_back(obj);
			setCurrNumber<T>(getCurrNumber<T>() + 1);
			setChanged<T>(true);
		}
		else
			ASSERT(false, "Drawer add() to unknown type $0", typeid(T).name());
	}

	template <typename T>
	void Drawer::clearImpl(StringId group)
	{
		if constexpr(std::is_same<T, Drawer::Line>::value || std::is_same<T, Drawer::Point>::value)
		{
			setCurrNumber<T>(getCurrNumber<T>() - getGroupsImpl<T>()[group.getId()].size());
			getGroupsImpl<T>()[group.getId()].clear();
			setChanged<T>(true);
		}
		else
			ASSERT(false, "Drawer clear() to unknown type $0. Should clear only lines or points.", typeid(T).name());
	}

	// Get data implementation
	template <typename T>
	std::vector<T>& Drawer::getImpl()
	{
		if constexpr(std::is_same<T, Drawer::Line>::value || std::is_same<T, Drawer::Point>::value)
		{
			if(getChanged<T>())
			{
				setCurrNumber<T>(0);

				std::map<StringHash, std::vector<T>> groups = getGroupsImpl<T>();
				for(auto& [key, group] : groups)
				{
					// Copy group objects to preallocated vector
					for(uint32_t i = 0; i < group.size(); i++)
					{
						if constexpr(std::is_same<T, Drawer::Line>::value)
							_lines[_currNumberOfLines++] = group[i];
						else if constexpr(std::is_same<T, Drawer::Point>::value)
							_points[_currNumberOfPoints++] = group[i];
					}
				}
				// Set changed as false
				setChanged<T>(false);
			}
		}

		if constexpr(std::is_same<T, Drawer::Line>::value)
			return _lines;
		else if constexpr(std::is_same<T, Drawer::Point>::value)
			return _points;
		else
			ASSERT(false, "Drawer get() to unknown type $0", typeid(T).name());
	}

	template <typename T>
	std::map<StringHash, std::vector<T>>& Drawer::getGroupsImpl()
	{
		if constexpr(std::is_same<T, Drawer::Line>::value)
			return _lineGroups;
		else if constexpr(std::is_same<T, Drawer::Point>::value)
			return _pointGroups;
		else
			ASSERT(false, "Drawer getGroups() to unknown type $0", typeid(T).name());
	}

	template <typename T>
	unsigned Drawer::getMaxNumberImpl()
	{
		if constexpr(std::is_same<T, Drawer::Line>::value)
			return _maxNumberOfLines;
		else if constexpr(std::is_same<T, Drawer::Point>::value)
			return _maxNumberOfPoints;
		else
			ASSERT(false, "Drawer getMaxNumber() to unknown type $0", typeid(T).name());
	}

	template <typename T>
	unsigned Drawer::getCurrNumberImpl()
	{
		if constexpr(std::is_same<T, Drawer::Line>::value)
			return _currNumberOfLines;
		else if constexpr(std::is_same<T, Drawer::Point>::value)
			return _currNumberOfPoints;
		else
			ASSERT(false, "Drawer getCurrNumber() to unknown type $0", typeid(T).name());
	}

	template <typename T>
	unsigned Drawer::getChanged()
	{
		if constexpr(std::is_same<T, Drawer::Line>::value)
			return _linesChanged;
		else if constexpr(std::is_same<T, Drawer::Point>::value)
			return _pointsChanged;
		else
			ASSERT(false, "Drawer getChanged() to unknown type $0", typeid(T).name());
	}

	template <typename T>
	void Drawer::setCurrNumber(unsigned number)
	{
		if constexpr(std::is_same<T, Drawer::Line>::value)
			_currNumberOfLines = number;
		else if constexpr(std::is_same<T, Drawer::Point>::value)
			_currNumberOfPoints = number;
		else
			ASSERT(false, "Drawer setCurrNumber() to unknown type $0", typeid(T).name());
	}

	template <typename T>
	void Drawer::setChanged(bool changed)
	{
		if constexpr(std::is_same<T, Drawer::Line>::value)
			_linesChanged = changed;
		else if constexpr(std::is_same<T, Drawer::Point>::value)
			_pointsChanged = changed;
		else
			ASSERT(false, "Drawer setChanged() to unknown type $0", typeid(T).name());
	}

	// Draw
	template <typename T>
	void Drawer::drawImpl()
	{ 
		if constexpr(std::is_same<T, Drawer::Line>::value)
		{
			glBindVertexArray(_lineVAO);
			if(_linesChanged)
			{
				// Update _line vector and gpu buffer
				getImpl<Drawer::Line>();
        		glBindBuffer(GL_ARRAY_BUFFER, _lineVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Line)*_currNumberOfLines, _lines.data());
			}
			glDrawArrays(GL_LINES, 0, 2*_currNumberOfLines);
			glBindVertexArray(0);
		}
		else if constexpr(std::is_same<T, Drawer::Point>::value)
		{
			glBindVertexArray(_pointVAO);
			if(_pointsChanged)
			{
				// Update _point vector and gpu buffer
				getImpl<Drawer::Point>();
        		glBindBuffer(GL_ARRAY_BUFFER, _pointVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point)*_currNumberOfPoints, _points.data());
			}
        	glDrawArrays(GL_POINTS, 0, _currNumberOfPoints);
			glBindVertexArray(0);
		}
		else
			ASSERT(false, "Drawer draw() to unknown type $0", typeid(T).name());
	}

	//---------- Line specific ----------//
	
	//---------- Point specific ----------//
}
