class UploadController < ApplicationController
  skip_before_filter :verify_authenticity_token
  
  def index
    
  end
  
  def uploadFile
    #render :text => params[:upload]['datafile'].original_filename;
#  data_file = params[:upload]['datafile'];
  
  itm = Item.new;

  uid = itm.store_to_disk (params[:upload]);
  itm.uploader_ip = request.env['REMOTE_ADDR'];


  itm.save;

	path = itm.path_to_file
 	filesize = File.size?(path);

        if (filesize == nil || filesize == 0)
		render :text => "FAIL";
		return;
        end

#	render :text => "FAIL";
#	return;

   redirect_to :controller => "upload", :action => "success" , :uploadID => uid;
    
  end
  
  def success
    @message = "http://htlr.org/" + params[:uploadID]; 
  end
end