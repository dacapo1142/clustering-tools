classdef AdjointSet<handle
    % A disjoint set of sets for clustering algorithm
    % The constructor accepts three arguments: n, k and which_cluster
    % n: The number of vertices
    % k: The number of clusters
    % which_cluster: A 1*n array where the n-th element equals to the belonging cluster id
    properties
        n
        k
        which_cluster
        next
        previous
        first
        last
        csize
    end
    methods
        function obj = AdjointSet(n,k,which_cluster)
            obj.n=n;
            obj.k=k;
            if nargin <=2
                which_cluster=mod(randperm(n),k)+1;
            end
            obj.which_cluster=which_cluster;
            initial(obj);
        end
        function initial(obj)
            obj.csize=zeros(1, obj.k);
            obj.first=-ones(1, obj.k);
            obj.last=-ones(1, obj.k);
            obj.next=-ones(1, obj.k);
            obj.previous=-ones(1,obj.k);
            for vid = 1:numel(obj.which_cluster),
                cid=obj.which_cluster(vid);
                insert(obj, vid, cid);
            end
        end
        function insert(obj, vid, cid)
            obj.csize(cid)=obj.csize(cid)+1;
            if obj.first(cid)==-1,
                obj.first(cid)=vid;
                obj.last(cid)=vid;
                obj.next(cid)=-1;
                obj.previous(cid)=-1;
            else
                last_vid=obj.last(cid);
                obj.last(cid)=vid;
                obj.previous(vid)=last_vid;
                obj.next(vid)=-1;
                obj.next(last_vid)=vid;
            end
        end
        function move(obj, vid, cid)
            %   move Move a vertex to another cluster
            %   move(vid, cid) move vertex vid to cluster cid
            old_cid=obj.which_cluster(vid);
            if old_cid==cid,
                return;
            else
                obj.csize(old_cid)=obj.csize(old_cid)-1;
                next_vid=obj.next(vid);
                previous_vid=obj.previous(vid);
                if previous_vid ~= -1,
                    obj.next(previous_vid)=next_vid;
                else
                    obj.first(old_cid)=next_vid;
                end
                if next_vid ~=-1,
                    obj.previous(next_vid)=previous_vid;
                else
                    obj.last(old_cid)=previous_vid;
                end
                insert(obj, vid, cid);
                obj.which_cluster(vid)=cid;
            end
        end
        function data=cluster(obj, cid)
            % return a 1*(cluster size) array, which contains the member vertices
            index=1;
            data=zeros(1, obj.csize(cid));
            vid=obj.first(cid);
            while vid~=-1,
                data(index)=vid;
                vid=obj.next(vid);
                index=index+1;
            end
        end


    end
end
