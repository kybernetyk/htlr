class SpyController < ApplicationController
  def index
    render :text => "PENISPUMPE"
  end
  
  def new
    all_items = Item.find(:all)
    
    output = "<table border=1 width=95%>"
    
    for itm in all_items
      output = output + "<tr><td>"
      output = output + itm.path_to_file
      output = output + "</td><td>"
      output = output + itm.content_type
      output = output + "</td><td>"

      output = output + "<a href='/" + itm.hosting_hash + "' target=_blank>";
      output = output + itm.hosting_hash;
      output = output + "</a>";
      output = output + "</td></tr>"
      
    end
    
    orutput = output +  "</table>"
    
    render :text => output
  end
  
end
