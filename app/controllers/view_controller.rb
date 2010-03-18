class ViewController < ApplicationController
#  caches_page :show

  def index
    
  end
  
  def show
    if params[:id] == nil
      render :action => :index
      return;
    end
    
   # database_id = Item.database_id_for_hosting_id(params[:id]);
    database_id = params[:id];
    if (database_id == nil)
      render :text => "ERROR! NOT FOUND LOL";
      return;
    end
    
    itm = Item.find_by_hosting_hash (database_id.to_s)
    
    if (itm == nil)
      render :text => "THE FILE NOT FOUNDS!"
      return;
    end
    
#    puts itm.inspect
    
    #render :text => itm.inspect
    
    send_file itm.path_to_file , :type => itm.content_type , :disposition => "inline"
    
  end

end
