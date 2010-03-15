class SpyController < ApplicationController
  def index
    render :text => "PENISPUMPE"
  end
  
  def new
    all_items = Item.find(:all)
    
    if (all_items == nil || !all_items)
      render :text => "NO ITAMZ"
      return
    end
      
    
    output = "<table border=1 width=95%>"
    
    for itm in all_items

      output = output + "<tr><td>"
      if (itm.path_to_file)
        output = output + itm.path_to_file
      end
      output = output + "</td>"

      output = output + "<td>"
      if (itm.content_type)
        output = output + itm.content_type
      end
      output = output + "</td>"

      output = output + "<td>"
      if (itm.hosting_hash)
        output = output + "<a href='/" + itm.hosting_hash + "' target=_blank>";
        output = output + itm.hosting_hash;
        output = output + "</a>";
      end
      output = output + "</td>";
      
      output = output + "</tr>"
      
    end
    
    orutput = output +  "</table>"
    
    render :text => output
  end
  
end
