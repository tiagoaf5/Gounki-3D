#include "YAFScene.h"
#include <iostream>
#include "cgfapplication.h"

YAFScene::YAFScene(char *filename) 
{
	this->filename = filename;
}

YAFScene::~YAFScene()
{
	delete(data);

	delete(doc);
	delete(globalsElement); 
	delete(camerasElement);
	delete(lightingElement);
	delete(texturesElement);
	delete(appearancesElement);
	delete(graphElement);

	map<string, CGFcamera *>::iterator it = camerasMap.begin();

	for(;it != camerasMap.end(); it++)
		delete(it->second);
}

bool YAFScene::readYaf(char *filename)
{
	doc=new TiXmlDocument(filename);
	bool loadOkay = doc->LoadFile();

	if (!loadOkay)
	{
		printf("Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc());
		exit(1);
	}

	TiXmlElement* yafElement= doc->FirstChildElement( "yaf" );

	if (yafElement == NULL)
	{
		printf("Main yaf block element not found! Exiting!\n");
		exit(1);
	}

	globalsElement = yafElement->FirstChildElement( "globals" );
	camerasElement = yafElement->FirstChildElement( "cameras" );
	lightingElement =  yafElement->FirstChildElement( "lighting" );
	texturesElement =  yafElement->FirstChildElement( "textures" );
	appearancesElement =  yafElement->FirstChildElement( "appearances" );
	animationsElement = yafElement->FirstChildElement( "animations" );
	graphElement =  yafElement->FirstChildElement( "graph" );

	//read globals block
	readGlobalsBlock();

	//read Cameras block
	readCamerasBlock();

	//lighting block
	readLightingBlock();

	//textures block
	readTexturesBlock();

	//appearances block
	readAppearancesBlock();

	//animations block
	readAnimationsBlock();

	//graph block
	readGraphBlock();
}

bool YAFScene::readGlobalsBlock ()
{
	if (globalsElement == NULL)
	{
		printf("Globals block not found!\n");
		exit(1);
	}
	else
	{
		printf("\nProcessing globals:\n");

		char *valString=NULL;
		float background[4];

		valString=(char *) globalsElement->Attribute("background");
		if(stringToArray(valString,4,background))
			printf("  Background values (r g b alfa): %f %f %f %f\n", background[0], background[1], background[2], background[3]);
		else
		{
			printf("Error parsing globals background\n");
			exit(1);
		}

		char *drawmode=NULL;
		char *shading=NULL;
		char *cullface=NULL;
		char *cullorder=NULL;

		drawmode=(char *) globalsElement->Attribute("drawmode");
		shading=(char *) globalsElement->Attribute("shading");
		cullface=(char *) globalsElement->Attribute("cullface");
		cullorder=(char *) globalsElement->Attribute("cullorder");

		if (drawmode == NULL || shading == NULL || cullface == NULL || cullorder == NULL)
		{
			printf("Error parsing globals arguments");
			exit(1);
		}
		else 
		{
			printf("  drawmode: %s\n", drawmode);
			printf("  shading: %s\n", shading);
			printf("  cullface: %s\n", cullface);
			printf("  cullorder: %s\n", cullorder);
		}
		data->setGlobals(background,drawmode,shading,cullface,cullorder);
	}

	return true;
}

bool YAFScene::readCamerasBlock ()
{
	if (camerasElement == NULL) 
	{
		printf("Cameras block not found!\n");
		exit(1);
	}
	else
	{
		printf("\nProcessing cameras:\n");

		char * initialCamera = NULL;

		initialCamera = (char *) camerasElement->Attribute("initial");

		if(initialCamera == NULL) 
		{
			printf("  Error parsing cameras arguments\n");
			exit(1);
		}
		else
			printf("  initial: %s\n", initialCamera);

		if(camerasElement->NoChildren()) 
		{
			printf("No cameras defined\n");
			exit(1);
		}
		else
		{
			TiXmlElement* camerasChildren = camerasElement->FirstChildElement();

			while (camerasChildren)
			{
				char * id = NULL;
				float nearV, farV;

				id = (char *) camerasChildren->Attribute("id"); 

				if (camerasChildren->QueryFloatAttribute("near",&nearV)==TIXML_SUCCESS &&
					camerasChildren->QueryFloatAttribute("far",&farV)==TIXML_SUCCESS && id != NULL)
				{

					if (strcmp(camerasChildren->Value(), "perspective") == 0)
					{
						float angle;
						char * pos = NULL;
						char * target = NULL;
						float position[3];
						float target1[3];

						MyPerspectiveCamera * mycam = NULL;

						pos = (char *) camerasChildren->Attribute("pos");
						target = (char *) camerasChildren->Attribute("target");

						if (camerasChildren->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS &&
							stringToArray(pos,3,position) && stringToArray(target,3,target1))
						{
							printf("  Perspective\n");
							printf("    id: %s\n", id);
							printf("    near: %f  far: %f  angle: %f\n",nearV,farV,angle);
							printf("    pos: %f %f %f\n",position[0],position[1],position[2]);
							printf("    target: %f %f %f\n",target1[0],target1[1],target1[2]);
						}
						else 
						{
							printf("Error parsing perspective arguments\n");
							exit(1);
						}

						mycam = new MyPerspectiveCamera(nearV,farV,angle,target1,position);
						addCamera(mycam, id);
					}
					else if (strcmp(camerasChildren->Value(), "ortho") == 0)
					{
						float left, right, top, bottom;
						MyOrthogonalCamera * mycam = NULL;

						if( camerasChildren->QueryFloatAttribute("left",&left)==TIXML_SUCCESS &&
							camerasChildren->QueryFloatAttribute("right",&right)==TIXML_SUCCESS &&
							camerasChildren->QueryFloatAttribute("top",&top)==TIXML_SUCCESS &&
							camerasChildren->QueryFloatAttribute("bottom",&bottom)==TIXML_SUCCESS)
						{
							printf("  Ortho\n");
							printf("    id: %s\n", id);
							printf("    near: %f  far: %f\n",nearV,farV);
							printf("    left: %f  right: %f\n",left,right);
							printf("    top: %f  bottom: %f\n",top,bottom);
						}
						else
						{
							printf("Invalid kind of camera: %s\n", camerasChildren->Value());
							exit(1);
						}
						mycam = new MyOrthogonalCamera(nearV, farV, left, right, top, bottom);
						addCamera(mycam,id);
					}
				}
				else
				{
					printf("Error parsing cameras' children");
					exit(1);
				}

				camerasChildren = camerasChildren->NextSiblingElement();
			}
			if(!setActiveCamera(initialCamera))
			{
				printf("Initial camera not defined\n");
				exit(1);
			}
		}
	}

	return true;
}

bool YAFScene::readLightingBlock ()
{
	int counter = 0;
	GLenum gLights[8];


	gLights[0] = GL_LIGHT0;
	gLights[1] = GL_LIGHT1;
	gLights[2] = GL_LIGHT2;
	gLights[3] = GL_LIGHT3;
	gLights[4] = GL_LIGHT4;
	gLights[5] = GL_LIGHT5;
	gLights[6] = GL_LIGHT6;
	gLights[7] = GL_LIGHT7;

	if (lightingElement == NULL) 
	{
		printf("Lighting block not found!\n");
		exit(1);
	}
	else
	{
		printf("\nProcessing lighting:\n");

		char *d = NULL;
		char *l = NULL;
		char *e = NULL;
		char *a = NULL;
		bool doublesided, local, enabled;
		float ambient[4]; //ambient

		d = (char *) lightingElement->Attribute("doublesided");
		l = (char *) lightingElement->Attribute("local");
		e = (char *) lightingElement->Attribute("enabled");
		a = (char *) lightingElement->Attribute("ambient");


		if (!stringToBool(d,doublesided) || !stringToBool(l,local) ||!stringToBool(e,enabled) 
			|| !stringToArray(a,4,ambient))
		{
			printf("Error parsing lighting arguments\n");
			exit(1);
		}
		else
		{
			printf("  doublesided: %s  local: %s  enabled: %s\n",d,l,e);
			printf("  ambient: ");

			for(unsigned int i = 0; i < 4; i++)
				printf("%f ",ambient[i]);
			printf("\n\n");
			data->setLighting(doublesided,local,enabled,ambient);
		}

		if(lightingElement->NoChildren()) 
		{
			printf("No lights defined\n");
			exit(1);
		}
		else
		{
			TiXmlElement* lightingChildren = lightingElement->FirstChildElement();
			vector<char * > names;
			while (lightingChildren)
			{
				Omnilight * o = NULL;
				SpotLight * s = NULL;
				if (strcmp(lightingChildren->Value(), "omni") == 0 || strcmp(lightingChildren->Value(), "spot") == 0)
				{
					bool isSpot = false;
					bool ok = true;

					char * id = NULL;
					char * enabled0 = NULL;

					char * location0 = NULL;				
					char * ambient0 = NULL;
					char * diffuse0 = NULL;
					char * specular0 = NULL;
					char * direction0= NULL;

					bool enabled1;
					float location1[3], ambient1[4], diffuse1[4], specular1[4], angle1, exponent1, direction1[3];
					float location2[4];

					id = (char *) lightingChildren->Attribute("id");
					enabled0 = (char *) lightingChildren->Attribute("enabled");

					location0 = (char *) lightingChildren->Attribute("location");
					ambient0 = (char *) lightingChildren->Attribute("ambient");
					diffuse0 = (char *) lightingChildren->Attribute("diffuse");
					specular0 = (char *) lightingChildren->Attribute("specular");

					if (strcmp(lightingChildren->Value(), "spot") == 0)
					{
						isSpot = true;
						direction0 = (char *) lightingChildren->Attribute("direction");

						if (lightingChildren->QueryFloatAttribute("angle",&angle1) != TIXML_SUCCESS
							|| lightingChildren->QueryFloatAttribute("exponent",&exponent1) != TIXML_SUCCESS
							|| !stringToArray(direction0, 3, direction1))
							ok = false;
					}

					if(id != NULL && stringToBool(enabled0,enabled1) &&
						stringToArray(location0,3, location1) && stringToArray(ambient0,4, ambient1) &&
						stringToArray(diffuse0,4, diffuse1) && stringToArray(specular0,4, specular1) && ok)
					{

						for (unsigned int i = 0 ; i < names.size(); i++)
							if ( strcmp(names[i], id) == 0)
							{
								printf("Light %s already exists\n",id);
								exit(1);
							}
							names.push_back(id);
							if (!isSpot){

								if (counter >= 7){
									printf("too many lights\n");
									exit(1);
								}

								location2[0] = location1[0];
								location2[1] = location1[1];
								location2[2] = location1[2];
								location2[3] = 1;


								o = new Omnilight(gLights[counter],enabled1,location2,ambient1,diffuse1,specular1);
								o->setAmbient(ambient1);
								o->setDiffuse(diffuse1);
								o->setSpecular(specular1);
								o->setIDS(id);

								data->addLight(o);
								counter++;
								printf("  Omni\n");
							}
							else{

								if (counter >= 7){
									printf("too many lights\n");
									exit(1);
								}

								location2[0] = location1[0];
								location2[1] = location1[1];
								location2[2] = location1[2];
								location2[3] = 1;

								s = new SpotLight(gLights[counter],enabled1,location2,ambient1,diffuse1,specular1,angle1,exponent1,direction1);

								s->setAmbient(ambient1);
								s->setDiffuse(diffuse1);
								s->setSpecular(specular1);
								s->setAngle(angle1);
								s->setIDS(id);

								data->addLight(s);
								counter++;
								printf("  Spot\n");
							}

							printf("    id: %s enabled: %s\n", id, enabled0);
							printf("    location: %f %f %f\n",location1[0],location1[1],location1[2]);
							printf("    ambient: %f %f %f %f\n",ambient1[0],ambient1[1],ambient1[2],ambient1[3]);
							printf("    diffuse: %f %f %f %f\n",diffuse1[0],diffuse1[1],diffuse1[2],diffuse1[3]);
							printf("    specular: %f %f %f %f\n",specular1[0],specular1[1],specular1[2],specular1[3]);

							if (isSpot) 
							{
								printf("    angle: %f\n", angle1);
								printf("    exponent: %f\n", exponent1);
								printf("    direction: %f %f %f\n",direction1[0],direction1[1],direction1[2]);
							}

					}
					else
					{
						printf("Error parsing light %s properties\n", id);
						exit(1);
					}
				}
				else
				{
					printf("Invalid kind of lighting: %s\n", lightingChildren->Value());
					exit(1);
				}

				lightingChildren = lightingChildren->NextSiblingElement();
			}
		}
	}

	return true;
}

bool YAFScene::readTexturesBlock ()
{

	if (texturesElement == NULL){
		printf("Texture block not found");
		exit(1);
	}

	else{
		printf("Processing textures:\n");

		if(texturesElement->NoChildren())
			printf("No textures defined\n");	
		else
		{
			TiXmlElement* textureChildren = texturesElement->FirstChildElement("texture");

			while (textureChildren){
				char *id=NULL;
				char *file=NULL;

				id=(char *) textureChildren->Attribute("id");
				file=(char *) textureChildren->Attribute("file");

				if (id == NULL || file == NULL) 
				{
					printf("Error parsing textures's arguments\n");
					exit(1);
				}
				else
				{
					printf("  id: %s\n", id);
					printf("  file: %s\n", file);
					data->addTextureToMap(id,file);
				}

				textureChildren = textureChildren->NextSiblingElement();
			}

		}
	}
	return true;
}

bool YAFScene::readAppearancesBlock ()
{
	if (appearancesElement == NULL) 
	{
		printf("Appearance block not found!\n");
		exit(1);
	}
	else{
		printf("Processing appearance:\n");

		TiXmlElement* appearanceChildren = appearancesElement->FirstChildElement("appearance");

		while (appearanceChildren)
		{
			char *id = NULL;
			char *textureref = NULL;
			MyAppearance * app = NULL;

			id = (char *) appearanceChildren->Attribute("id");
			textureref = (char *) appearanceChildren->Attribute("textureref");


			if (id == NULL)
			{
				printf("Error parsing appearances arguments\n");
				exit(1);
			}
			else
			{
				float angle;
				char *emi = NULL;
				char *amb = NULL;
				char *dif = NULL;
				char *spec = NULL;
				char *shin = NULL;
				char *t_s = NULL;
				char *t_t = NULL;

				float emissive[4];
				float ambient[4];
				float diffuse[4];
				float specular[4];
				float shi;
				float tex_s;
				float tex_t;

				emi = (char *) appearanceChildren->Attribute("emissive");
				amb = (char *) appearanceChildren->Attribute("ambient");
				dif = (char *) appearanceChildren->Attribute("diffuse");
				spec = (char *) appearanceChildren->Attribute("specular");
				shin = (char *) appearanceChildren->Attribute("shininess");
				t_s = (char *) appearanceChildren->Attribute("texlength_s");
				t_t = (char *) appearanceChildren->Attribute("texlength_t");

				if( stringToArray(emi,4,emissive) &&
					stringToArray(amb,4,ambient) &&
					stringToArray(dif,4,diffuse) &&
					stringToArray(spec,4,specular) &&
					appearanceChildren->QueryFloatAttribute("shininess",&shi)==TIXML_SUCCESS)
				{
					printf("  Appearance\n");
					printf("    id: %s\n    emissive: ", id);

					for(unsigned int i = 0; i < 4; i++)
						printf("%f ",emissive[i]);
					printf("\n    ambient:  ");

					for(unsigned int i = 0; i < 4; i++)
						printf("%f ",ambient[i]);
					printf("\n    diffuse:  ");

					for(unsigned int i = 0; i < 4; i++)
						printf("%f ",diffuse[i]);
					printf("\n    specular: ");

					for(unsigned int i = 0; i < 4; i++)
						printf("%f ",specular[i]);
					printf("\n");

					printf("	shin: %f\n",shi);

					app = new MyAppearance(ambient,diffuse,specular,emissive,shi); 
					app->setId(id);

					if ( textureref != NULL)
					{
						if (appearanceChildren->QueryFloatAttribute("texlength_s",&tex_s)==TIXML_SUCCESS &&
							appearanceChildren->QueryFloatAttribute("texlength_t",&tex_t)==TIXML_SUCCESS)
						{
							printf("	t_r: %s\n",textureref);
							printf("	t_s: %f\n",tex_s);
							printf("	t_t: %f\n",tex_t);
						}
						else
						{
							printf("Error parsing texture in appearance\n");
							exit(1);
						}
						app->setTexLength(tex_s, tex_t);
						app->setTextureWrap(GL_REPEAT,GL_REPEAT);
						app->setTextureId(textureref);
					}

					data->addAppearanceToMap(id,app);
				}
				else
				{
					printf("Error parsing appearance arguments\n");
					exit(1);
				}
			}

			appearanceChildren = appearanceChildren->NextSiblingElement();
		}
	}
	return true;
}


bool YAFScene::readAnimationsBlock(){

	if (animationsElement == NULL){
		printf("Animations block not found!\n");
		exit(1);
	}
	else
	{
		printf("Processing animations:\n");


		TiXmlElement* animationsChildren = animationsElement->FirstChildElement("animation");

		while (animationsChildren)
		{
			char *id = NULL;
			char *span = NULL;
			char *type = NULL;

			float span1;

			id = (char *) animationsChildren->Attribute("id");
			span = (char *) animationsChildren->Attribute("span");
			type = (char *) animationsChildren->Attribute("type");

			if (id == NULL || animationsChildren->QueryFloatAttribute("span",&span1) != TIXML_SUCCESS ||
				type == NULL)
			{
				printf("Error parsing appearances arguments\n");
				exit(1);
			}

			else 
			{
				if(strcmp(type,"linear") != 0 )
				{
					printf("Type %s not supported. Only linear type supported \n");
					exit(1);
				}
				if(span1 < 0)
				{
					printf("Span must be positive\n");
					exit(1);
				}

				printf("Animation id: %s\n", id);
				printf(" span: %f\n",span1);
				printf(" type: %s\n",type);
			}

			TiXmlElement * controlpoint = animationsChildren->FirstChildElement("controlpoint");
			int n_controlpoints = 0;
			vector<vector<double>> controlpoints;
			while(controlpoint)
			{
				if(controlpoint)
				{
					char *xx = NULL;
					char *yy = NULL; 
					char *zz = NULL;

					float xx1;
					float yy1;
					float zz1;

					vector<double> p;

					xx = (char *)controlpoint->Attribute("xx");
					yy = (char *)controlpoint->Attribute("yy");
					zz = (char *)controlpoint->Attribute("zz");

					if (controlpoint->QueryFloatAttribute("xx",&xx1) != TIXML_SUCCESS|| controlpoint->QueryFloatAttribute("yy",&yy1) != TIXML_SUCCESS 
						|| controlpoint->QueryFloatAttribute("zz",&zz1))
					{
						printf("Error parsing  arguments\n");
						exit(1);
					}
					else
					{
						n_controlpoints++;
						printf("  Control point %d\n", n_controlpoints);
						printf("   xx: %f\n",xx1);
						printf("   yy: %f\n",yy1);
						printf("   zz: %f\n",zz1);
						p.push_back(xx1);
						p.push_back(yy1);
						p.push_back(zz1);
					}

					controlpoints.push_back(p);
				}
				controlpoint = controlpoint->NextSiblingElement();
			}
			if(n_controlpoints < 2)
			{
				printf("Error reading control points, expected at least 2 received %d\n",n_controlpoints);
				exit(1);
			}
			//add animation to map
			LinearAnimation * la = new LinearAnimation(controlpoints,span1);
			data->addAnimationToMap(id,la);
			animationsChildren = animationsChildren->NextSiblingElement();
		}
	}
	return 0;
}


bool YAFScene::readGraphBlock ()
{
	if (graphElement == NULL) 
	{
		printf("Graph block not found!\n");
		exit(1);
	}
	else
	{
		printf("\nProcessing Graph:\n");
		char * rootid = NULL;

		rootid = (char *) graphElement->Attribute("rootid");

		if (rootid == NULL)
		{
			printf("Rootid not defined\n");
			exit(1);
		}

		data->addNodeToMap(rootid,NULL); //adds root to map
		data->setRootStr(rootid);


		if(graphElement->NoChildren()) {
			printf("No nodes defined\n");
			exit(1);
		}

		TiXmlElement* node = graphElement->FirstChildElement("node");

		while (node)
		{
			if(node)
			{
				char * id = NULL;
				char * displayList = NULL;

				bool dList;

				id = (char *) node->Attribute("id");
				displayList = (char*) node->Attribute("displaylist");

				if (id == NULL ){
					printf("  Error parsing node argument\n");
					exit(1);
				}
				else
				{
					printf("  Node id: %s\n", id);

					Node * mynode = new Node(id);
					if(displayList != NULL) 
					{
						if (stringToBool(displayList,dList))
						{
							printf("  DisplayList : %s\n",displayList);
							if(dList)
								mynode->setDisplaylist();
						}
						else
						{
							printf("Invalid parameter for displaylist\n");
							exit(1);
						}
					}

					TiXmlElement* transform = node->FirstChildElement("transforms")->FirstChildElement();

					if(transform) {
						printf("    Transforms\n");

					}

					glLoadIdentity();
					while(transform)
					{
						//printf("--->%s\n",transform->Value());

						if (strcmp(transform->Value(), "translate") == 0)
						{
							float x,y,z;
							char * xyz;

							xyz = (char *) transform->Attribute("to");

							if( sscanf(xyz,"%f %f %f",&x,&y,&z) == 3)
								printf("      translate: %f %f %f\n",x,y,z);
							else{
								printf("	  invalid Translation\n");
								exit(1);
							}

							glTranslatef(x,y,z);

						}
						else if (strcmp(transform->Value(), "rotate") == 0)
						{
							char * axis = NULL;
							float angle;

							axis = (char *) transform->Attribute("axis");

							if (transform->QueryFloatAttribute("angle",&angle) == TIXML_SUCCESS && 
								(strcmp(axis,"x") == 0|| strcmp(axis,"y") == 0 || strcmp(axis,"z") == 0))
								printf("      rotate: %s %f\n",axis,angle);
							else
							{
								printf("		invalid Rotation\n");
								exit(1);
							}

							if (strcmp(axis,"x") == 0)
								glRotatef(angle,1,0,0);
							else if (strcmp(axis,"y") == 0)
								glRotatef(angle,0,1,0);
							else if (strcmp(axis,"z") == 0)
								glRotatef(angle,0,0,1);

						}
						else if (strcmp(transform->Value(), "scale") == 0)
						{
							float x,y,z;
							char * xyz;

							xyz = (char *) transform->Attribute("factor");

							if( sscanf(xyz,"%f %f %f",&x,&y,&z) == 3)
								printf("      scale: %f %f %f\n",x,y,z);
							else{
								printf("	  invalid scaling\n");
								exit(1);
							}

							glScalef(x,y,z);
						}
						else {
							printf("      Invalid transformation\n");
							exit(1);
						}

						transform = transform->NextSiblingElement();
					}


					float m1[4][4];
					glGetFloatv(GL_MODELVIEW_MATRIX, &m1[0][0]);
					mynode->addTransforms(m1);

					//END transforms
					TiXmlElement* appearance = node->FirstChildElement("appearanceref");

					if(appearance)
					{
						char * appId = NULL;

						appId = (char *) appearance->Attribute("id");

						if (appId != NULL)
							printf("    Appearanceref id: %s\n", appId);
						else 
						{
							printf("      Invalid Appearanceref\n");
							exit(1);
						}
						mynode->setAppearanceStr(appId);
					}

					TiXmlElement* animation = node->FirstChildElement("animationref");

					if(animation)
					{
						char *animId = NULL;

						animId = (char*) animation->Attribute("id");

						if (animId != NULL)
						{
							printf("    Animationref id: %s\n",animId);
							mynode->setAnimationStr(animId);
						}
						else
						{
							printf("	Invalid Animationref\n");
							exit(1);
						}
					}

					TiXmlElement* children = node->FirstChildElement("children")->FirstChildElement();


					if(children)
					{
						printf("    Children:\n");
						while(children)
						{
							if (strcmp(children->Value(), "rectangle") == 0)
							{
								float x1,y1,x2,y2;
								char * xy1;
								char * xy2;

								xy1 = (char *) children->Attribute("xy1");
								xy2 = (char *) children->Attribute("xy2");

								if( sscanf(xy1,"%f %f",&x1,&y1) == 2 &&
									sscanf(xy2,"%f %f",&x2,&y2) == 2)
								{
									printf("      Rectangle\n");
									printf("		x1:%f y1:%f\n",x1,y1);
									printf("		x2:%f y2:%f\n",x2,y2);
								}
								else {
									printf("invalid Rectangle\n");
									exit(1);
								}

								mynode->addprimitive(new MyRectangle(x1,y1,x2,y2, this));

							}
							else if (strcmp(children->Value(), "triangle") == 0)
							{
								float x1,y1,z1,x2,y2,z2,x3,y3,z3;
								char * xyz1;
								char * xyz2;
								char * xyz3;

								xyz1 = (char *) children->Attribute("xyz1");
								xyz2 = (char *) children->Attribute("xyz2");
								xyz3 = (char *) children->Attribute("xyz3");

								if( sscanf(xyz1,"%f %f %f",&x1,&y1,&z1) == 3 &&
									sscanf(xyz2,"%f %f %f",&x2,&y2,&z2) == 3 &&
									sscanf(xyz3,"%f %f %f",&x3,&y3,&z3) == 3){
										printf("      Triangle:\n		x1:%f y1:%f z1:%f\n",x1,y1,z1);
										printf("		x2:%f y2:%f z2:%f\n",x2,y2,z2);
										printf("		x3:%f y3:%f z3:%f\n",x3,y3,z3);
								}
								else{
									printf("invalid triangle\n");
									exit(1);
								}

								mynode->addprimitive(new Triangle(x1,y1,z1,x2,y2,z2,x3,y3,z3,this));
							}


							else if (strcmp(children->Value(), "cylinder") == 0)
							{
								float b,t,h;
								int sli,sta;
								char * base;
								char * top;
								char * height;
								char * slices;
								char * stacks;

								base = (char *) children->Attribute("base");
								top = (char *) children->Attribute("top");
								height = (char *) children->Attribute("height");
								slices = (char *) children->Attribute("slices");
								stacks = (char *) children->Attribute("stacks");

								if( children->QueryFloatAttribute("base",&b) == TIXML_SUCCESS &&
									children->QueryFloatAttribute("top",&t) == TIXML_SUCCESS &&
									children->QueryFloatAttribute("height",&h) == TIXML_SUCCESS &&
									children->QueryIntAttribute("slices",&sli) == TIXML_SUCCESS &&
									children->QueryIntAttribute("stacks",&sta) == TIXML_SUCCESS)
								{
									printf("      Cylinder\n");
									printf("		base:%f\n",b);
									printf("		top:%f\n",t);
									printf("		height:%f\n",h);
									printf("		slices:%i\n",sli);
									printf("		stacks:%i\n",sta);
									mynode->addprimitive(new Cylinder(b,t,h,sli,sta));
								}
								else{
									printf("invalid cylinder\n");
									exit(1);
								}
							}
							else if (strcmp(children->Value(), "sphere") == 0)
							{
								float r;
								int sli,sta;
								char * radius;
								char * slices;
								char * stacks;

								radius = (char *) children->Attribute("radius");
								slices = (char *) children->Attribute("slices");
								stacks = (char *) children->Attribute("stacks");

								if( children->QueryFloatAttribute("radius",&r) == TIXML_SUCCESS &&
									children->QueryIntAttribute("slices",&sli) == TIXML_SUCCESS &&
									children->QueryIntAttribute("stacks",&sta) == TIXML_SUCCESS){

										printf("      Sphere\n");
										printf("		radius:%f\n",r);
										printf("		slices:%i\n",sli);
										printf("		stacks:%i\n",sta);

										mynode->addprimitive(new Sphere(r,sli,sta));
								}
								else{
									printf("invalid Sphere\n");
									exit(1);
								}
							}


							else if (strcmp(children->Value(), "torus") == 0)
							{
								float in,out;
								int sli,loo;
								char * inner;
								char * outer;
								char * slices;
								char * loops;

								inner = (char *) children->Attribute("inner");		
								outer = (char *) children->Attribute("outer");
								slices = (char *) children->Attribute("slices");
								loops = (char *) children->Attribute("loops");

								if( children->QueryFloatAttribute("inner",&in) == TIXML_SUCCESS &&
									children->QueryFloatAttribute("outer",&out) == TIXML_SUCCESS &&
									children->QueryIntAttribute("slices",&sli) == TIXML_SUCCESS &&
									children->QueryIntAttribute("loops",&loo) == TIXML_SUCCESS){

										printf("      Torus\n");
										printf("		inner:%f\n",in);
										printf("		outer:%f\n",out);
										printf("		slices:%i\n",sli);
										printf("		loops:%i\n",loo);

										mynode->addprimitive(new Torus(in,out,sli,loo));
								}

								else{
									printf("invalid Torus\n");
									exit(1);
								}
							}

							else if (strcmp(children->Value(),"plane") == 0){

								char * parts;
								int parts1;

								parts = (char *) children->Attribute("parts");

								if ( children->QueryIntAttribute("parts",&parts1) == TIXML_SUCCESS){
									printf("      Plane\n");
									printf("		parts:%i\n",parts1);
									mynode->addprimitive(new Plane(parts1, this));

								}
								else{
									printf("invalid plane\n");
									exit(1);
								}

							}

							else if (strcmp(children->Value(),"patch") == 0){

								char * order;
								char * partsU;
								char * partsV;
								char * compute;

								int order1;
								int partsU1;
								int partsV1;
								int compute1;
								int nrPoints = 0;

								order = (char*) children->Attribute("order");
								partsU = (char*) children->Attribute("partsU");
								partsV = (char*) children->Attribute("partsV");
								compute = (char*) children->Attribute("compute");


								if ( children->QueryIntAttribute("order",&order1) == TIXML_SUCCESS &&
									children->QueryIntAttribute("partsU",&partsU1) == TIXML_SUCCESS &&
									children->QueryIntAttribute("partsV",&partsV1) == TIXML_SUCCESS &&
									compute != NULL)
								{
									printf("      Patch\n");
									printf("		order:%i\n",order1);
									printf("		partsU1:%i\n",partsU1);
									printf("		partsV1:%i\n",partsV1);
									printf("		compute:%s\n",compute);


									TiXmlElement* controlpoint = node->FirstChildElement("children")->FirstChildElement("patch")->FirstChildElement();

									vector<vector<float> > vectorPoints;
									while(controlpoint){
										float x,y,z;
	
										if(controlpoint->QueryFloatAttribute("x",&x) == TIXML_SUCCESS &&
										   controlpoint->QueryFloatAttribute("y",&y) == TIXML_SUCCESS &&
										   controlpoint->QueryFloatAttribute("z",&z) == TIXML_SUCCESS){

											   vector<float> pontos;
											  
											   printf("		x:%f",x);
											   printf("	y:%f",y);
											   printf("	z:%f\n",z);

											   pontos.push_back(x);
											   pontos.push_back(y);
											   pontos.push_back(z);

											   vectorPoints.push_back(pontos);
											   nrPoints++;
										}
										else{
											printf("error parsing Controlpoint coordinates\n");
											exit(1);
										}
										
										controlpoint = controlpoint->NextSiblingElement();
									}

									if (order1 == 1){
										if (nrPoints != 4){
											printf("wrong number of controlpoints\n");
											exit(1);
										}
									}

									else if (order1 == 2){
										if (nrPoints != 9){
											printf("wrong number of controlpoints\n");
											exit(1);
										}
									}

									else if (order1 == 3){
										if (nrPoints != 16){
											printf("wrong number of controlpoints\n");
											exit(1);
										}
									}

									mynode->addprimitive(new Patch(order1,partsU1,partsV1,compute,vectorPoints,this));
								}
								else{
									printf("invalid patch\n");
									exit(1);
								}

							}

							else if (strcmp(children->Value(),"vehicle") == 0){

								printf("      Vehicle exists!\n");
								mynode->addprimitive(new Vehicle(this));

							}

							else if(strcmp(children->Value(),"waterline") == 0)
							{

								char *heightmap;
								char *texturemap;
								char *fragmentshader;
								char *vertexshader;

								heightmap = (char*) children->Attribute("heightmap");
								texturemap = (char*) children->Attribute("texturemap");
								fragmentshader = (char*) children->Attribute("fragmentshader");
								vertexshader = (char*) children->Attribute("vertexshader");

								if (heightmap != NULL || texturemap != NULL || fragmentshader != NULL || vertexshader != NULL){

									printf("      Waterline\n");
									printf("		heightmap:%s\n",heightmap);
									printf("		texturemap: %s\n",texturemap);
									printf("		fragmentshader: %s\n",fragmentshader);
									printf("		vertexshader: %s\n",vertexshader);
									Waterline * wat = new Waterline(texturemap,heightmap,vertexshader,fragmentshader,this);
									data->setShader(wat->getShader());
									mynode->addprimitive(wat);
								}
							}

							else if(strcmp(children->Value(),"clock") == 0){
								printf("		Clock Exists\n");
								myClock * c1 = new myClock(this);
								mynode->addprimitive(c1);
								data->setClock(c1);
							}

							if (strcmp(children->Value(), "noderef") == 0)
							{

								char * noderefId = NULL;

								noderefId = (char *) children->Attribute("id");

								if (noderefId != NULL) {
									printf("      Noderef id: %s\n", noderefId);
									mynode->addChildStr(noderefId);
									data->addNodeToMap(noderefId, NULL);
								}
								else
								{
									printf("invalid noderef\n");
									exit(1);
								}
							}

							children = children->NextSiblingElement();
						}
					}
					else
					{
						printf("no children!\n");
						exit(1);
					}

					data->addNodeToMap(id,mynode);
				}
			}
			else
			{
				printf("Invalid graph's child\n");
				exit(1);
			}

			node = node->NextSiblingElement();

		}
	}

	return true;
}


bool YAFScene::stringToBool (char * str, bool & value) 
{
	if(str == NULL)
		return false;

	if (strcmp(str,"true")==0) {
		value = true;
	}
	else if (strcmp(str,"false")==0)
		value = false;
	else
		return false;

	return true;
}

bool YAFScene::stringToArray (char * str, int nParam, float values[]) //change to stringstream
{

	switch (nParam)
	{
	case 2:
		if (sscanf(str,"%f %f",&values[0],&values[1]) == nParam)
			return true;
		else 
			return false;
		break;
	case 3:
		if (sscanf(str,"%f %f %f",&values[0],&values[1],&values[2]) == nParam)
			return true;
		else 
			return false;
	case 4:
		if (sscanf(str,"%f %f %f %f",&values[0],&values[1],&values[2],&values[3]) == nParam)
			return true;
		else 
			return false;
	default:
		return false;
	}
}

Data * YAFScene::getData(){
	return data;
}

void YAFScene::init()
{
	data = new Data();
	glMatrixMode(GL_MODELVIEW);
	readYaf(filename);

	//>work out the appearances
	data->computeAppearancePointers();
	//data->printAppearancesMap();

	//>work out the nodes
	//data->printNodesMap();
	data->computeNodePointers();

	//taking care of display lists
	data->doBfs();
	data->computeDisplayLists();

	//>init socket
	//data->initSocket();

	//> Picking
	data->initPicking();

	//Game
	data->initGame();
	//Board
	//data->initBoard();

	//>test the textures
	//data->printTexturesMap();

	//> Defines a default normal
	glNormal3f(0,0,1);

	//> Defining globals
	data->applyGlobals();
	data->applyLighting(); //apply lighting options, doesn't change during display


	glEnable(GL_COLOR_MATERIAL);
	setUpdatePeriod(30);
	glEnable(GL_NORMALIZE);

	//enable alpha channel
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//setUpdatePeriod(100);
}

void YAFScene::update(unsigned long t)
{
	vector<Animation *> animations = data->getAnimations();
	for(unsigned int i = 0; i < animations.size(); i++)
		animations[i]->update(t);
	vector<MyShader *> shaders = data->getShaders();

	for (unsigned int i = 0; i < shaders.size(); i++)
		shaders[i]->update(t);
	((myClock *)(data->getClock()))->update(t);
}


void YAFScene::display()
{
	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//apply camera
	applyCamera();

	//set globals
	data->applyGlobals();

	//set lights
	data->applyLights();

	//draw axis
	axis.draw();

	//draw scene
	data->drawScene();

	//////////////////
	glutSwapBuffers();
	glFlush();
}

void YAFScene::displaySelect()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	applyCamera();
	data->drawSceneSelect();
}


void YAFScene::addCamera(CGFcamera * cam, string camid)
{
	if (camerasMap.find(camid) != camerasMap.end())
	{
		cout << "Camera " << camid << " already defined" << endl;
		exit(1);
	}
	camerasMap.insert(pair<string, CGFcamera *>(camid,cam));
}

bool YAFScene::setActiveCamera(string camid)
{

	if (camid.empty())
	{
		CGFscene::activateCamera(0);
		CGFapplication::activeApp->forceRefresh();
		return true;
	}

	map<string, CGFcamera *>::iterator it = camerasMap.find(camid);

	if (it == camerasMap.end())
		return false;
	else
	{
		//activeCamera = it->second;
		//myscene->setActiveCamera(it->second);
		CGFscene::activeCamera =  it->second;
		CGFapplication::activeApp->forceRefresh();
		return true;
	}
}

void YAFScene::setActiveCamera(CGFcamera * cam) {
	CGFscene::activeCamera = cam;
}

void YAFScene::applyCamera()
{
	activeCamera->applyView();
}


CGFcamera * YAFScene::getActiveCamera() const
{
	return CGFscene::activeCamera;
}

map<string, CGFcamera *> YAFScene::getCamerasMap() const
{
	return camerasMap;
}
