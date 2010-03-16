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

   redirect_to :controller => "upload", :action => "success" , :id => uid;
    
  end
  
  def success
    item = Item.find_by_hosting_hash (params[:id])

    if (!item)
      render :text => "FAIL"
      return;
    end

    @message = "http://htlr.org/" + params[:id] + "\n<p>"
    @message = @message + "Upload successful. Share your upload through this URL: "
    @message = @message + "<a href='/" + params[:id] + "'>http://htlr.org/" + params[:id] + "</a>"; 
    @message = @message + "\n<p>"
    
    if (item.content_type.include?("image") || item.content_type.include?("img"))
      @message = @message + "<img src='/" + item.hosting_hash + "'>"
    else
      @message = @message + "<a href='/" + item.hosting_hash + "'>" + item.original_filename + "</a>"
    end
    

    
  end
end
