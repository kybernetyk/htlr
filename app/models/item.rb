class Item < ActiveRecord::Base

def self.hosting_id_for_string (string_or_file)
    require 'zlib'
    
    the_id = Zlib.adler32(string_or_file,0);
    puts "database id = " + the_id.to_s;
    
    the_id = the_id.to_s(23)
     
    return the_id;
    
end
  
def self.database_id_for_hosting_id (hosting_id)
  #the_id = hosting_id.to_i(23)
  #return the_id;
  return hosting_id
end

def store_to_disk (upload_post_data)
    name =  upload_post_data['datafile'].original_filename; 
  
    self.original_filename = name;
    self.content_type = upload_post_data['datafile'].content_type;
    
    directory = "public/data/" + rand(0xffff + 1).to_s;

    name = name.gsub(" ","_");

    #create the dir if it's missing
    Dir.mkdir(directory) unless File.directory?(directory) 

    # create the file path
    path = File.join(directory, name)
    # write the file
    File.open(path, "wb") { |f| f.write(upload_post_data['datafile'].read) }
 
    
    self.path_to_file = path;

    #hosting_id = Item.hosting_id_for_string(directory);
    #self.id = Item.database_id_for_hosting_id(hosting_id);
    
    
    hosting_id = Item.hosting_id_for_string(self.path_to_file);
    self.hosting_hash = Item.database_id_for_hosting_id(hosting_id).to_s;

  #  self.user_ip = request.env['REMOTE_ADDR'];

    return hosting_id;
    
#   return directory.gsub("public/","") + "/" + name
    
  end

  def self.random
      ids = connection.select_all("SELECT id FROM items")
      find(ids[rand(ids.length)]["id"].to_i) unless ids.blank?
  end


end
