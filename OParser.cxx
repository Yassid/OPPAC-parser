#include "OParser.h"




template <typename T1, typename T2>
herr_t parse_sim_to_h5(
  MapFileType& map,
  const char* const hdf_output_filename,
  std::vector<T1>& xpos,
  std::vector<T2>& ypos,
  const std::vector<int>& diodes)
{

	//SiPM X-Y matrix
	double3 histgrid(boost::extents[map.size()][diodes.size()][1000]);
	std::fill_n(histgrid.data(), histgrid.num_elements(), 0);
	std::size_t file_cnt =0;
	std::vector<int> events;

     
	for(MapFileType::const_iterator it = map.begin(); it != map.end(); ++it)
  	{

			std::string sim_file_name = it->first;
			std::ifstream sim_file(sim_file_name, std::ifstream::in);

			//std::cout<< xpos.size()<<"	"<<ypos.size()<<"\n";
			events.clear();

			std::vector<double> sipm_buffer;
			int sipmcnt = 0;
			std::size_t evt_num = 0;

			xpos[file_cnt]=it->second.first;
			ypos[file_cnt]=it->second.second;

			std::cout<<" Opening and processing file : "<<sim_file_name<<"\n";
			std::cout<<" X pos "<<xpos[file_cnt]<<" Y pos "<<ypos[file_cnt]<<"\n";
				

				std::string buffer;

				while (!sim_file.eof())
			    	{
					sim_file>>buffer;
					if(std::stoi(buffer)==9999){
						 
						 events.push_back(evt_num);
						 //std::cout<<" Event number "<<evt_num<<" file "<<sim_file_name<<"\n";

						 for (std::size_t k=0; k<sipm_buffer.size(); k++){
							//std::cout<< sipm_buffer.size()<<"	"<<sipmcnt <<"\n";
							//assert(sipm_buffer.size()==(diodes.size()*4));
							histgrid[file_cnt][k][evt_num] = sipm_buffer[k];
					
						 }// SiPM loop (k)

						++evt_num;

						 sipm_buffer.clear();
						 sipmcnt = 0;

					}else{
						 //std::cout<<buffer<<"\n";
						 sipm_buffer.push_back(std::stoi(buffer));
						 ++sipmcnt;
					}
				}
		

		    sim_file.close();
		    ++file_cnt;
		    

		}//file map

		    return write_h5(hdf_output_filename,map,xpos,ypos,histgrid,diodes,events);
	

}

herr_t write_h5(
  const char* const filename,
  MapFileType& map,
  const std::vector<double>& xpos,
  const std::vector<double>& ypos,
  const double3& histgrid,
  const std::vector<int>& diodes,
  const std::vector<int>& events
) {
  
	std::cout<<histgrid.shape()[2]<<"	"<<events.size()<<"\n";

  assert(histgrid.shape()[0] == map.size());
  assert(histgrid.shape()[1] == diodes.size());
  assert(histgrid.shape()[2] == events.size()-1);

  const hsize_t xpos_dims[1] = {xpos.size()};
  const hsize_t ypos_dims[1] = {ypos.size()};
  const hsize_t num_diodes_dims[1] = {diodes.size()};
  const hsize_t events_dims[1] = {events.size()};
  const hsize_t files_dims[1] = {map.size()};
  const hsize_t histgrid_dims[3] = {files_dims[0], num_diodes_dims[0],events_dims[0]};

  std::cout<<files_dims[1]<<"	"<<xpos_dims[0]<<"	"<<ypos_dims[0]<<"	"<<num_diodes_dims[0]<<"	"<<events_dims[0]<<"\n";

   hid_t h5_file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  H5LTmake_dataset_double(h5_file_id, "Xpos", 1, xpos_dims,xpos.data());
  H5LTmake_dataset_double(h5_file_id, "Ypos", 1, ypos_dims, ypos.data());
  H5LTmake_dataset_int(h5_file_id, "Num_diodes", 1, num_diodes_dims, diodes.data());
  H5LTmake_dataset_double(h5_file_id, "histgrid", 3, histgrid_dims, histgrid.data());

 

  herr_t h5_status = H5Fclose(h5_file_id);
  return h5_status;

		

}

int main(int argc, char *argv[])
{

	if(argc==1)
	{
		std::cout<<" Wrong nuimber of parameters "<<"\n";
		std::cout<<" Usage: ./OParser path_to_file_with_filename"<<"\n";
		std::cout<<" Exiting ..."<<"\n";
		return 0;  
	}

	std::ifstream dataFiles(argv[1]);
	std::vector<std::string> files;
	std::string dataFileWithPath;
	std::map<std::string,std::pair<double,double>> file_map;
	std::vector<double> xpos;
        std::vector<double> ypos;

	while(std::getline(dataFiles,dataFileWithPath)) 
		files.push_back(dataFileWithPath);


	boost::regex regex("simu_x([-]?\\d+)_y([-]?\\d+)");

	//Count files and positions
	for(std::size_t iFile = 0; iFile<files.size();++iFile)
	{
	std::cout<<" File "<<files[iFile]<<"\n";
	
	      boost::smatch match;
	      if (boost::regex_search(files[iFile], match, regex))
	      {
		
		
		std::cout<<" File found "<<files[iFile]<<" "<<match[2]<<" "<<match[1]<<"\n";
		auto coords = std::make_pair<double,double>(std::stod(match[1]),std::stod(match[2]));
		file_map[files[iFile]] = coords;
		xpos.push_back(std::stod(match[1]));
		ypos.push_back(std::stod(match[2]));
	      }  
	   

	  }
        

	int num_diodes = 33*4;

	const char* const hdf_output_filename = "simu_HDF.h5";


	const auto diodes = make_vector<int>(1,num_diodes+1,1);
	

        parse_sim_to_h5(file_map,hdf_output_filename,xpos,ypos,diodes);


	return 0;


	


	

}


