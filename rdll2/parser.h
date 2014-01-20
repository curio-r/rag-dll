
enum CEffect {
	CEFFECT_NONE = 0,
	CEFFECT_SKY,
	CEFFECT_SNOW,
	CEFFECT_MAPLE,
	CEFFECT_SAKURA,
	CEFFECT_POKJUK,
	CEFFECT_NIGHT
};

typedef struct {
	char* name;
	unsigned int clearColor;
	CEffect effectType;
	unsigned int param;
} mapConf;

typedef struct {
	mapConf* entries;
	unsigned int count;
} confData;

const unsigned int SIZE_CONF_ENTRIES = 100;

confData* parseConfFile() {

	confData* cdata = (confData*)malloc(sizeof(confData));

	cdata->entries = (mapConf*)malloc(SIZE_CONF_ENTRIES * sizeof(mapConf));
	cdata->count = 0;

	char* confFileName = "data/mapeffecttable.txt";

	FILE* confFile = fopen(confFileName, "r");
	
	if(confFile == 0) {
		MessageBoxA(NULL, "Failed to load data/mapeffecttable.txt. Client defaults will be used.", "", MB_OK);
		return NULL;
	}
	
	char line[1024];
	
	char mapname[1024];
	unsigned int color = 0;
	char effect[1024];
	unsigned int param = 0;

	while(fgets(line, sizeof(line), confFile)) {
		
		if(strstr(line, "//") == line || strlen(line) < 4)
			continue;
		
		sscanf(line, "%s\t%x\t%s\t%i", mapname, &color, effect, &param);
		
		mapConf* mc = &(cdata->entries[cdata->count]);

		mc->name = _strdup(mapname);
		mc->clearColor = color;
		
		CEffect ceff;
		
		if(_strcmpi(effect, "sky") == 0)
			ceff = CEFFECT_SKY;
		else if(_strcmpi(effect, "snow") == 0)
			ceff = CEFFECT_SNOW;
		else if(_strcmpi(effect, "maple") == 0)
			ceff = CEFFECT_MAPLE;
		else if(_strcmpi(effect, "sakura") == 0)
			ceff = CEFFECT_SAKURA;
		else if(_strcmpi(effect, "pokjuk") == 0)
			ceff = CEFFECT_POKJUK;
		else if(_strcmpi(effect, "night") == 0)
			ceff = CEFFECT_NIGHT;
		else
			ceff = CEFFECT_NONE;

		mc->effectType = ceff;
		mc->param = param;

		cdata->count++;

	}

	return cdata;
}

// Global config
confData* G_CONFDATA;

mapConf* getMapConfig(char* mapname) {

	mapConf* mc = NULL;

	for(unsigned int i = 0; i < G_CONFDATA->count; i++) {
		if(_strcmpi(G_CONFDATA->entries[i].name, mapname) == 0) {
			mc = &(G_CONFDATA->entries[i]);
		}
	}

	if(mc == NULL) {
		mc = &(G_CONFDATA->entries[0]);
	}

	return mc;

}
