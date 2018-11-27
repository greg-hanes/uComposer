//#include "Project.h"
//#include "VectorUtility.h"
//
//namespace uComposer
//{
//	Project::Project(AudioDeviceInterface *deviceInterface) :
//		m_deviceInterface(deviceInterface)
//	{
//	}
//
//	Composition *Project::CreateComposition(const char *name)
//	{
//		Composition *composition = new Composition(name);
//		m_compositions.push_back(composition);
//		return composition;
//	}
//
//	const std::vector<Composition *> &Project::Compositions() const
//	{
//		return m_compositions;
//	}
//
//	void Project::DeleteComposition(Composition *composition)
//	{
//		VecUtil::Delete(m_compositions, composition);
//	}
//
//	DLL_EXTERN Project *Project_Construct(AudioDeviceInterface *deviceInterface)
//	{
//		return Project::CreateNewProject(deviceInterface);
//	}
//
//	DLL_EXTERN Project *Project_Load(AudioDeviceInterface *deviceInterface, const char *path)
//	{
//		return Project::LoadProject(deviceInterface);
//	}
//
//	DLL_EXTERN void Project_Delete(Project *project)
//	{
//		delete project;
//	}
//
//	//DLL_EXTERN Composition *Project_CreateComposition(Project *project)
//	//{
//	//	return project->CreateComposition();
//	//}
//
//	//DLL_EXTERN size_t Project_GetCompositionCount(Project *project)
//	//{
//	//	return project->GetCompositionCount();
//	//}
//	//DLL_EXTERN Composition **Project_GetCompositions(Project *project)
//	//{
//	//	return project->GetCompositions();
//	//}
//	//DLL_EXTERN Composition *Project_GetComposition(Project *project, int index)
//	//{
//	//	return project->GetCompositions()[index];
//	//}
//	//DLL_EXTERN void Project_DeleteComposition(Project *project, Composition *composition)
//	//{
//	//	project->DeleteComposition(composition);
//	//}
//}