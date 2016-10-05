function [ collections, objective ] = ksetsplus(g, n, k, varargin)
pnames={'measure','verbose', 'which_cluster'};
dflts={'distance',false,[]};
[measure, vervose, which_cluster]=internal.stats.parseArgs(pnames, dflts, varargin{:});

addpath('containers');
if which_cluster
    collections=AdjointSet(n,k,which_cluster);
else
    collections=AdjointSet(n,k);
end
iter_order=1:n;
changed=true;
count=0;
while changed
    count=count+1;
    changed=false;
    for vid=iter_order
        min_adjust_distance=inf;
        old_cid=collections.which_cluster(vid);
        if collections.csize(old_cid) ==1
            continue;
        end
        
        for cid=1:k
            cluster=collections.cluster(cid);
            cluster_size=collections.csize(cid);
            switch measure
                case 'cohesion'
                    delta_distance=g(vid,vid)...
                        -2/cluster_size*sum(g(vid, cluster))...
                        +1/cluster_size^2*sum(sum(g(cluster,cluster)));
                case 'distance'
                    delta_distance=...
                        (2*cluster_size*sum(g(vid,cluster)/cluster_size)...
                        -sum(sum(g(cluster,cluster)))/cluster_size^2);
            end
            
            if cid==old_cid
                adjust_distance=delta_distance*cluster_size/(cluster_size-1);
            else
                adjust_distance=delta_distance*cluster_size/(cluster_size+1);
            end
            
            if adjust_distance<min_adjust_distance
                min_adjust_distance=adjust_distance;
                best_cid=cid;
            end
        end
        
        if best_cid ~= old_cid
            collections.move(vid, best_cid);
            changed=true;
        end
    end
    switch measure
        case 'distance'
            objective=0;
            %             for cid=1:k
            %                 csize=collections.csize(k);
            %                 cluster=collections.cluster(k);
            %                 gss=g(cluster,cluster);
            %                 gss=sum(gss(:));
            %                 %objective=objective+2*gss/csize-gss/csize^2;
            %             end
            for vid=1:n
                cid=collections.which_cluster(vid);
                csize=collections.csize(cid);
                cluster=collections.cluster(cid);
                delta_distance=2*sum(g(vid, cluster))/csize-sum(sum(g(cluster, cluster)))/csize^2;
                objective=objective+delta_distance;
            end
            disp(objective);
    end
end

