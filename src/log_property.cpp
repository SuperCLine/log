#include "stdafx.h"
#include "log_property.h"


__BEGIN_NAMESPACE

struct log_property_impl
{
	bool m_asyn;
	ulist<log_property::logger_type> m_logger_list;
	umap<ustring, bool> m_filter_map;
	umap<ustring, bool> m_tag_map;

	log_property_impl() : m_asyn(false)
	{}
};

log_property::log_property(void)
: m_impl(new log_property_impl())
{
	
}

log_property::~log_property(void)
{
	app_safe_delete(m_impl);
}

ustring log_property::serialize(void)
{
	Json::Value root;
	Json::Value logger_array;
	Json::Value logger_item;

	logger_item["type"] = ELoggerType::ELT_CONSOLE;
	logger_item["name"] = "";
	logger_array.append(logger_item);

	logger_item["type"] = ELoggerType::ELT_FILE_DEBUG;
	logger_item["name"] = "debug.log";
	logger_array.append(logger_item);

	logger_item["type"] = ELoggerType::ELT_FILE_INFO;
	logger_item["name"] = "info.log";
	logger_array.append(logger_item);

	logger_item["type"] = ELoggerType::ELT_FILE_ERROR;
	logger_item["name"] = "err.log";
	logger_array.append(logger_item);

	logger_item["type"] = ELoggerType::ELT_FILE_PERF;
	logger_item["name"] = "perf.log";
	logger_array.append(logger_item);
	
	Json::Value filter;
	filter["debug"] = true;
	filter["info"] = true;
	filter["err"] = true;
	filter["perf"] = true;

	Json::Value tag;
	tag.append("core");
	tag.append("pathfinder");
	tag.append("lua");

	root["asyn"] = false;
	root["logger"] = logger_array;
	root["filter"] = filter;
	root["tag"] = tag;

#if defined(DEBUG) || defined(_DEBUG)
	//Json::StyledWriter writer;
	//return writer.write(root);

	Json::StreamWriterBuilder builder;
	return Json::writeString(builder, root);
#else
	//Json::FastWriter writer;
	//return writer.write(root);

	Json::StreamWriterBuilder builder;
	return Json::writeString(builder, root);
#endif

}

bool log_property::deserialize(const ustring& name)
{
	std::ifstream fs(name.c_str());
	if (!fs.is_open())
		return false;

	Json::CharReaderBuilder builder;
	builder["collectComments"] = true;

	bool ret = false;
	Json::Value root;
	JSONCPP_STRING errs;
	if (parseFromStream(builder, fs, &root, &errs))
	{
		m_impl->m_asyn = root["asyn"].asBool();

		const Json::Value& logger = root["logger"];
		const Json::Value& filter = root["filter"];
		const Json::Value& tag = root["tag"];

		for (uint32 i = 0; i < logger.size(); ++i)
		{
			logger_type data;
			data.type = (ELoggerType)logger[i]["type"].asInt();
			data.name = logger[i]["name"].asString();
			m_impl->m_logger_list.push_back(data);
		}

		m_impl->m_filter_map["debug"] = filter["debug"].asBool();
		m_impl->m_filter_map["info"] = filter["info"].asBool();
		m_impl->m_filter_map["err"] = filter["err"].asBool();
		m_impl->m_filter_map["perf"] = filter["perf"].asBool();

		for (uint32 i = 0; i < tag.size(); ++i)
		{
			m_impl->m_tag_map[tag[i].asString()] = true;
		}

		ret = true;
	}

	fs.close();

	return ret;
}

bool log_property::is_asyn(void)
{
	return m_impl->m_asyn;
}

const ulist<log_property::logger_type>& log_property::get_logger_list(void)
{
	return m_impl->m_logger_list;
}

umap<ustring, bool>& log_property::get_filter_map(void)
{
	return m_impl->m_filter_map;
}

umap<ustring, bool>& log_property::get_tag_map(void)
{
	return m_impl->m_tag_map;
}

__END_NAMESPACE
