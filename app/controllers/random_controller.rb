class RandomController < ApplicationController
  def index
    item = Item.random
    
    if (item.content_type.include?("image") || item.content_type.include?("img"))
      @message = "<img src='/" + item.hosting_hash + "'>"
    else
      @message = "<a href='/" + item.hosting_hash + "'>" + item.original_filename + "</a>"
    end
  end
end
